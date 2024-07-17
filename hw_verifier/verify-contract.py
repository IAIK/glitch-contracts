import argparse
import logging
import sys
from CircuitGraph import CircuitGraph
import time
import networkx as nx
from ComplianceChecker import ComplianceChecker
from defines import *
from classes import *
import helpers
from ContractCfg import ContractCfg

def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Verify whether a processor complies with the specifications of a side-channel contract",
        fromfile_prefix_chars="@")
    parser.add_argument("-dis", "--disassembler", dest="path_disassembler",
                        required=False, type=helpers.ap_check_file_exists,
                        help="Path to disassembler plugin with 'disassemble(instr: int)' function")
    parser.add_argument("-cfg", "--config", dest="path_contract_configs",
                        nargs="+", required=True, type=helpers.ap_check_file_exists,
                        help="Path of the config files, specifying available registers in contract and HW"
                             ", public wires, and simulatable values.")
    parser.add_argument("-cb", "--contract-base", dest="path_contract_base",
                        required=True,
                        help="Base path of the smt2 files which contain the encoding of the contract.")
    parser.add_argument("-instr", "--list-instr", dest="list_executableInstr",
                        required=False, default=False, action='store_true',
                        help="List (some) instructions which are executable in HW despite constraints and "
                             "contract (default: %(default)s)")
    parser.add_argument("-invar", "--invariant", dest="verify_invariant",
                        required=False, default=False, action='store_true',
                        help="Whether to verify the correctness of mappings (default: %(default)s)")
    parser.add_argument("-prop", "--propagation", dest="verify_propagation",
                        required=False, default=False, action='store_true',
                        help="Whether to verify the correctness of wire mappings (default: %(default)s)")
    parser.add_argument("-equiv", "--equivalence", dest="verify_equivalence",
                        required=False, default=False, action='store_true',
                        help="Whether to verify the correctness of register (default: %(default)s)")
    parser.add_argument("-comb", "--combination", dest="verify_combination",
                        required=False, default=False, action='store_true',
                        help="Whether to verify the correctness of leakages (default: %(default)s)")
    parser.add_argument("-j", "--json", dest="json_file_path",
                        required=True, type=helpers.ap_check_file_exists,
                        help="Path of output JSON file (default: %(default)s)")
    parser.add_argument("-c", "--cycles", dest="cycles",
                        required=False, type=int, default=1,
                        help="Number of cycles / instructions to verify (default: %(default)s)")
    parser.add_argument("-m", "--mode", dest="mode",
                        required=False, default=GLITCH, choices=[STABLE, TRANSIENT, GLITCH],
                        help="The verification mode (default: %(default)s)")
    parser.add_argument("-dnodes", "--debug-nodes", dest="debug_nodes",
                        nargs="+", required=False, default=None, type=int,
                        help="restrict verification to a set of hardware nodes for debugging (default: %(default)s)")

    parser.add_argument("-n", "--num-leaks", dest="num_leaks",
                        required=False, type=int, default=1,
                        help="Number of leakage locations to be reported if the circuit is insecure. (default: %(default)s)")
    parser.add_argument("-o", "--output", dest="output",
                        required=False, default=TMP_DIR + "/model",
                        help="Base path in which counterexamples are written (default: %(default)s)")
    parser.add_argument("-d", "--dbg-output-dir", dest="dbg_output_dir_path",
                        required=False, default=TMP_DIR,
                        help="Directory in which debug traces (dbg-label-trace-?.txt, dbg-circuit-?.dot) are written (default: %(default)s)")
    parser.add_argument("-dbg", "--debug", dest="debug",
                        required=False, default=False, action='store_true',
                        help="Display debugging information (default: %(default)s)")
    parser.add_argument("--log", dest="fnlog",
                        required=False, type=str, default=None,
                        help="file name of log file (default: %(default)s)")
    parser.add_argument("-top", "--top-module", dest="top_module",
                        required=True, type=str,
                        help="Name of the hardware designs top module")
    parser.add_argument("-p", "--processes", dest="processes",
                        required=False, type=int, default=1,
                        help="Verify using one or multiple processes (default: %(default)s)")
    parser.add_argument("-so", "--solver", dest="solver",
                        required=False, type=str, default='z3', choices=['z3', 'btor'],
                        help="Use specified solver (default: %(default)s)")
    parser.add_argument("-th", "--theories", dest="theories",
                        required=False, type=str, default='any', choices=['any', 'QF_BV'],
                        help="Use optimized solver for specified theories (default: %(default)s)")
    args = parser.parse_args()
    if args.cycles <= 0:    args.cycles = UINT_MAX
    if args.num_leaks <= 0: args.num_leaks = UINT_MAX
    if args.processes <= 0: args.processes = max(os.cpu_count() - 2, 1)

    #Unfortunately, ap_check_dir_exists does not work for optional parameters
    helpers.check_dir_exists(args.dbg_output_dir_path)

    return args


def main():
    args = parse_arguments()

    # configure logging
    level = logging.DEBUG if args.debug else logging.INFO
    logging.basicConfig(filename=args.fnlog, format='%(asctime)s [%(levelname)s] %(message)s', datefmt='%Y-%m-%d %H:%M:%S', level=level)
    
    print('args:', args)

    cfg = ContractCfg(args.path_contract_configs, args.json_file_path, args.top_module, args.cycles, debug_nodes=args.debug_nodes)
    assert(cfg is not None)

    compliant = None
    if args.list_executableInstr:
        checker = ComplianceChecker(cfg, args)
        checker.list_executable_instrunstions()
        del checker

    if args.verify_invariant:
        checker = ComplianceChecker(cfg, args)
        compliant = checker.check_invariant()
        del checker
        if compliant:
            logging.info("Hardware obeys invariants defined in the contract!")
        else:
            logging.info("Hardware does not comply with the invariants defined in the contract!")

    if args.verify_equivalence:
        checker = ComplianceChecker(cfg, args)
        compliant = checker.check_reg_mappings()
        del checker

        if compliant:
            logging.info("Hardware is architecturally compliant!")
        else:
            logging.info("Hardware is architecturally BROKEN")

    if args.verify_propagation:
        checker = ComplianceChecker(cfg, args)
        compliant = checker.check_propagation()
        del checker

        if compliant:
            logging.info("Hardware obeys propagation defined in the contract!")
        else:
            logging.info("Hardware does not comply with propagation defined in the contract!")

    if args.verify_combination:
        checker = ComplianceChecker(cfg, args)
        compliant = checker.check_combination()
        del checker

        if compliant:
            logging.info("Hardware obeys combinations defined in the contract!")
        else:
            logging.info("Hardware does not comply with the combinations defined in the contract!")

    if compliant is None:
        logging.error("No verification was performed. Set the appropriate switch to start verification.")
        sys.exit(EXIT_CODE_INPUT_ERR)
    elif compliant:
        sys.exit(EXIT_CODE_COMPLIANT)
    else:
        sys.exit(EXIT_CODE_NONCOMPLIANT)

if __name__ == "__main__":
    main()
