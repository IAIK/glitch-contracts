import functools
import importlib.util
import itertools
import json
import logging
import sys
from datetime import datetime
from os import path

import helpers
import networkx as nx
import parallel
import smt
from CircuitGraph import CONST_TO_BIT, CircuitGraph
from defines import *
from SafeGraph import SafeGraph
from collections import defaultdict
import defines
from classes import SignalConstraint, CfgZ3Typ


def smt_xnor(x, y):
    return smt.Not(smt.Xor(x, y))


def smt_extract_bool(z3_bv, pos):
    return smt.Equals(smt.Extract(pos, pos, z3_bv), smt.BitVecVal(1, 1))


def smt_list_xor(lst):
    r = smt.BoolVal(False)
    for li in lst:
        r = smt.Xor(r, li)
    return r


def smt_cell_identifier(cell, cycle):
    return "%s!%d!@%d" % (cell.name, cell.pos, cycle)


def parse_ctrct_name_map(fn):
    sw_map = {}
    with open(fn, "r") as f:
        map_data = f.read().strip().split("\n")

    ln = 1
    for line in map_data:
        # skip empty lines
        if len(line) == 0:
            ln += 1
            continue

        src, dst = line.strip().split(" ", 1)
        if " " in src or " " in dst:
            logging.error(f"line {ln} of {fn}: invalid directive in mapping directive")
            sys.exit(EXIT_CODE_INPUT_ERR)

        if src in sw_map:
            logging.error(f"line {ln} of {fn}: {src} duplicate mapping!")
            sys.exit(EXIT_CODE_INPUT_ERR)

        sw_map[src] = dst

    return sw_map


class ComplianceChecker:
    def __init__(self, cfg, args):
        self.debug = args.debug
        self.storeartifacts = True
        self.output = args.output
        self.sanitycheck = True # sanity check that configuration is satisfiable
        self.dryrun = False
        self.dryrun_prop_specified_reg = True # TODO self.dryrun
        self.dryrun_prop_specified_map = True # TODO self.dryrun
        self.dryrun_prop_specified_pub = self.dryrun
        self.dryrun_prop_specified_lst = self.dryrun
        self.dryrun_prop_simulatable = self.dryrun
        self.dryrun_prop_find_public = self.dryrun
        self.dryrun_prop_find_simulators = self.dryrun
        self.dryrun_leak_stable = self.dryrun
        self.dryrun_leak_trans = self.dryrun
        if self.dryrun:
            logging.warning("dry run, no real verification running! all results void!")
        self.vp = args.verify_propagation
        self.vc = args.verify_combination
        self.sequential = args.processes <= 1
        self.processes = args.processes
        self.cfg = cfg
        self.max_cexes = 10000
        assert args.mode in (TRANSIENT, STABLE, GLITCH)
        self.mode = args.mode

        assert args.cycles >= 1
        self.ncycles = args.cycles
        self.preCycle = False

        self.counterexamplecounter = 0

        circuit_graph = CircuitGraph(json.load(open(args.json_file_path, "r")), args.top_module)
        self.circuit = SafeGraph(circuit_graph.graph)

        self.num_leaks             = args.num_leaks
        self.dbg_output_dir_path   = args.dbg_output_dir_path
        self.path_contract_base    = args.path_contract_base
        self.path_contract_configs = args.path_contract_configs

        self.solver = smt.create_solver(args.solver, args.theories)
        #if args.theories == 'QF_BV':
        #    self.solver = smt.Then('simplify', 'solve-eqs', 'qfbv').solver()
        #else:
        #    self.solver = smt.Solver()
        # self.solver = smt.Then('simplify', 'solve-eqs', 'qfufbv').solver()
        # self.solver = smt.Tactic("smtfd").solver()
        # self.solver = smt.SolverFor("QF_UFDT")
        # self.solver = smt.SolverFor("QF_UFBV")
        self.node_pvars: dict[int, dict[str, smt.ExprRef]]  = {}  # cycle -> pvars {}
        self.node_simulators: dict[int, set[smt.ExprRef]] = {}  # node -> set(creg)
        self.ctrct_substitutes = {}
        self.leak_statements: dict[int, list] = {}
        
        self.glitch_gate_stability: dict[int, smt.ExprRef] = defaultdict(lambda: defaultdict(lambda: smt.BoolVal(True)))
        # cycle -> node -> expr
        #self.glitch_gate_delta: dict[int, dict[int, smt.ExprRef]] = defaultdict(lambda: defaultdict(lambda: defaultdict(lambda: [])))
        self.glitch_gate_delta: dict[int, dict[int, smt.ExprRef]] = defaultdict(lambda: defaultdict(lambda: smt.BoolVal(False)))
        # cycle -> node -> expr

        self.real_consts = {"0": smt.BoolVal(False), "1": smt.BoolVal(True),
                            "x": smt.Bool("_z3_internal_x"), "z": smt.Bool("_z3_internal_z")}

        self.disassembler = None
        if args.path_disassembler:
            spec = importlib.util.spec_from_file_location("module.name", args.path_disassembler)
            self.disassembler = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(self.disassembler)

    def __del__(self):
        self.solver.reset()
        del self.solver
        del self

    def build_hwmodel_cycle(self, cycle, first_cycle):
        assert (self.solver.num_scopes() == 0), "There are unclosed scopes"
        if self.sanitycheck:
            assert (self.solver.check() == smt.sat), "HW circuit unfolding incorrect before cycle %d" % cycle
        assert(cycle not in self.node_pvars.keys())
        if cycle == first_cycle:
            # prepare checking of compliance
            self.__init_smt_vars(cycle)
        else:
            # link output state of last cycle with this one
            self.__trans_registers(cycle)
        # unfold the circuit once
        self.__build_hw_smt(cycle)
        if cycle != first_cycle and self.mode == GLITCH:
            self.__build_glitch_smt(cycle, forward_activations=(cycle > 0))
        if self.sanitycheck:
            assert (self.solver.check() == smt.sat), "HW circuit unfolding incorrect after cycle %d" % cycle

    def z3_bitvec_op(self, name, ins, op):
        assert(len(ins) != 0)
        l = len(ins[0])
        for in_ in ins: assert(len(in_) == l)

        # special case: if both inputs are same in each context
        special = all(map(lambda in_: all(map(lambda ini: ini.eq(in_[0]), in_)), ins))
        if special:
            ret = [smt.Bool(name) for _ in range(l)]
            xs = [in_[0] for in_ in ins]
            self.solver.add(smt.Equals(ret[0], op(*xs)))
            return ret

        ret = [smt.Bool("%s__%d" % (name, i)) for i in range(l)]
        for i in range(l):
            xs = [in_[i] for in_ in ins]
            self.solver.add(smt.Equals(ret[i], op(*xs)))
        return ret

    @staticmethod
    def __def_hw_vars(cycle, cell_id, cell, curr_pvars):
        name = smt_cell_identifier(cell, cycle)
        curr_pvars[cell_id] = [smt.Bool(f"{name}__{i}") for i in range(NUM_CONTEXTS)]

    @staticmethod
    def __def_hw_pubs(cycle, cell_id, cell, curr_pvars):
        prop_const = smt.Bool(smt_cell_identifier(cell, cycle))
        # add the same value to each context, as leakages of observable cells may be connected to this state
        curr_pvars[cell_id] = [prop_const for _ in range(NUM_CONTEXTS)]

    def __init_hw_name(self, cycle, name, curr_pvars, fun):
        cell_ids = self.cfg.hw_name_ids[name]
        for cell_id in cell_ids:
            cell = self.circuit.cells[cell_id]
            if cell.type == CONST_TYPE: continue
            if (cell_id in curr_pvars):
                for name in cell.names:
                    if name in self.cfg.hw_state_pub:
                        logging.warning(f"duplicate {name}")
                assert False, f'{name} already in pvars'
            assert (cell.type in REGPORT_TYPES), f"invalid annotation for {[self.circuit.cells[i] for i in cell_ids]}"
            fun(cycle, cell_id, cell, curr_pvars)

    def __init_smt_vars(self, cycle):
        # ensure that init has not yet been called
        assert(len(self.node_pvars) == 0)
        curr_pvars = {}

        # add constants used in circuit representation
        for r in CONST_TO_BIT.keys():
            curr_pvars[CONST_TO_BIT[r]] = [self.real_consts[r] for _ in range(NUM_CONTEXTS)]

        # declare hw variables in smt
        for hvar in self.cfg.hw_state_var:
            self.__init_hw_name(cycle, hvar, curr_pvars, ComplianceChecker.__def_hw_vars)

        # declare hw constants in smt
        for hcon in self.cfg.hw_state_pub:
            self.__init_hw_name(cycle, hcon, curr_pvars, ComplianceChecker.__def_hw_pubs)

        # declare all unknowns as constants as well and give warnings
        for cell_id in self.circuit.cells:
            if cell_id in curr_pvars: continue
            cell = self.circuit.cells[cell_id]
            if cell.type not in REGPORT_TYPES: continue
            alias = ["%s[%d]" % (n, cell.names[n]) for n in cell.names]
            logging.warning("Declaring public " + (", ".join(alias)))
            self.__def_hw_pubs(cycle, cell_id, cell, curr_pvars)

        self.node_pvars[cycle] = curr_pvars

    """ link the result state of the previous cycle with the current cycle
    """
    def __trans_registers(self, cycle):
        assert(cycle not in self.node_pvars and max(self.node_pvars.keys()) == cycle - 1)
        prev_pvars = self.node_pvars[cycle - 1]

        curr_pvars = {}
        # add constants used in circuit representation
        for r in CONST_TO_BIT.keys():
            curr_pvars[CONST_TO_BIT[r]] = [self.real_consts[r] for _ in range(NUM_CONTEXTS)]

        # define the value of each internal register in this cycle by
        # using the outcome from the previous cycle [cycle-1]
        for node_id in self.circuit.nodes:
            cell = self.circuit.cells[node_id]
            if cell.type not in tuple(REGISTER_TYPES):
                continue
            preds = tuple(self.circuit.predecessors(node_id))
            assert(len(preds) == 1)
            in_pvars = prev_pvars[preds[0]]
            # add assertions that the register has the same value as
            # the input of the register of the previous cycle
            res_pvars = self.z3_bitvec_op(smt_cell_identifier(cell, cycle), [in_pvars], lambda x: x)
            curr_pvars[node_id] = res_pvars

        # add the values of (external) ports, i.e. input to the circuit in cycle [cycle]
        for node_id in self.circuit.nodes:
            cell = self.circuit.cells[node_id]
            if cell.type != PORT_TYPE: continue

            # if the port was previously a variable,
            #   then define a new variable for this cycle
            #   else define a new constant for this cycle
            node_prev_pvars = prev_pvars[node_id]
            head, rest = node_prev_pvars[0], node_prev_pvars[1:]
            was_const = all(map(lambda pn: head.eq(pn), rest))
            if was_const:
                self.__def_hw_pubs(cycle, node_id, cell, curr_pvars)
            else:
                self.__def_hw_vars(cycle, node_id, cell, curr_pvars)

        # add the new register values to cycle [cycle]
        self.node_pvars[cycle] = curr_pvars

    def __build_hw_smt(self, cycle):
        OP_LOOKUP = {XOR_TYPE: smt.Xor, XNOR_TYPE: smt_xnor,
                     AND_TYPE: smt.And, OR_TYPE: smt.Or}
        # we only construct wires here and the registers and ports must already be defined
        assert(max(self.node_pvars.keys()) == cycle)
        curr_pvars = self.node_pvars[cycle]

        # represent the resulting value of each gate as smt definition
        for node_id in self.circuit.nodes:
            cell = self.circuit.cells[node_id]

            # if self.debug: logging.debug("Building", cell)

            res_pvars = None
            preds = tuple(self.circuit.predecessors(node_id))

            # match on the kind of gate/cell
            if cell.type in GATE_TYPES:

                # check that no definitions have been added for this node so far
                if (node_id in curr_pvars):
                    logging.error(f"circuit node {node_id} already in observable values: {cell}")
                    sys.exit(EXIT_CODE_UNREACHABLE)

                res_pvars = self.z3_bitvec_op(smt_cell_identifier(cell, cycle),
                                              (curr_pvars[preds[0]], curr_pvars[preds[1]]),
                                              OP_LOOKUP[cell.type])

            elif cell.type == NOT_TYPE:
                # check that no definitions have been added for this node so far
                if (node_id in curr_pvars):
                    logging.error(f"circuit node {node_id} already in observable values: {cell}")
                    sys.exit(EXIT_CODE_UNREACHABLE)
                assert(preds[0] in curr_pvars)
                res_pvars = self.z3_bitvec_op(smt_cell_identifier(cell, cycle), (curr_pvars[preds[0]],), smt.Not)

            elif cell.type in tuple(REGISTER_TYPES) + (PORT_TYPE, CONST_TYPE):

                # check that a definition was made for this cell
                if not(node_id in curr_pvars):
                    logging.error(f"circuit node {node_id} has not been defined previously: {cell}")
                    sys.exit(EXIT_CODE_UNREACHABLE)
                # skip loop iteration to prevent adding further definitions
                continue

            elif cell.type == MUX_TYPE:

                # check that no definitions have been added for this node so far
                if (node_id in curr_pvars):
                    logging.error(f"circuit node {node_id} already in observable values: {cell}")
                    sys.exit(EXIT_CODE_UNREACHABLE)

                x_pvars = curr_pvars[cell.mux_ins[0]]
                y_pvars = curr_pvars[cell.mux_ins[1]]
                s_pvars = curr_pvars[cell.select]
                res_pvars = self.z3_bitvec_op(smt_cell_identifier(cell, cycle), (s_pvars, y_pvars, x_pvars), smt.If)
                     
            else:
                logging.error(f"unexpected kind of circuit node {node_id}")
                sys.exit(EXIT_CODE_UNREACHABLE)

            # confirm that some action took place
            assert(res_pvars is not None)

            # add the definition for this node to the global definitions
            curr_pvars[node_id] = res_pvars

        # print the solver contents
        if self.storeartifacts:
            with open(f"{self.path_contract_base}hwmodel_{cycle}.smt2", "w+") as f:
                f.write(self.solver.to_smt2())
                
    def __find_glitch_registers(self, cycle):
        hw_regs = [nid for nid in self.circuit.nodes if self.circuit.cells[nid].type in REGISTER_TYPES]
        hw_regs_secret = set(hw_regs)
        
        print('len', len(hw_regs))
        
        publics = set()
        for cell_id, cregs in self.cfg.state_map_cellsw.items():
            cell_id = int(cell_id)
            if cell_id not in hw_regs:
                continue
            if len(cregs) == 1 and cregs[0] == "public":
                hw_regs_secret.remove(cell_id)
                publics.add(cell_id)
            
        covered = set()
        self.glitch_registers = {}
        key_fun = (lambda x: (x[0].startswith("_") & 1) * 100000 - len(x[1]))
        wire_order = sorted(self.cfg.hw_name_ids.items(), key=key_fun)
        for hw_name, bits in wire_order:
            if all(map(lambda x: (x in hw_regs_secret), bits)) and not covered.issuperset(bits):
                print(f"nonpublic wire {hw_name} (size {len(bits)})")
                self.glitch_registers[hw_name] = list(bits)
                covered.update(bits)
                
        remaining = hw_regs_secret - covered
        logging.info(f'private register wires found: {len(self.glitch_registers)}, remaining register cells: {len(remaining)}')
        for i, nid in enumerate(remaining):
            logging.warn(f'nonpublic register cell (node_id={i}) without corresponding wire found')
            self.glitch_registers[f'__pseudo_wire_{i}'] = [nid]
            
                
        #print('len(self.glitch_registers)', len(self.glitch_registers))
        #print('len(covered)', len(covered))
        #print('len(hw_regs_secret)', len(hw_regs_secret))
        #print('len(hw_regs_secret - covered)', len(hw_regs_secret - covered))
            
        #print('len2', len(self.cfg.state_map_cellsw.items()))
        #print('len covered', len(covered))
                
        #assert covered == set(hw_regs), 'some cells could not be processed' # TODO check that all registeres have been checked
        
        #print('len(hw_regs)', len(hw_regs))
        #sys.exit()

    def __gate_stability(self, cell, node_id, curr_pvars, prev_pvars, stable, preds):
        # only used for stable values -> other context can be ignored
        w = 0
        match cell.type:
            case defines.CONST_TYPE:
                r = smt.BoolVal(True)
            case defines.NOT_TYPE:
                r = stable[preds[0]]
            case defines.XOR_TYPE | defines.XNOR_TYPE:
                r = smt.And(stable[preds[0]], stable[preds[1]])
            case defines.AND_TYPE:
                r = smt.Or(smt.And(stable[preds[0]], stable[preds[1]]),
                            smt.And(stable[preds[0]], smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(False))),
                            smt.And(stable[preds[1]], smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(False))))
            case defines.OR_TYPE:
                r = smt.Or(smt.And(stable[preds[0]], stable[preds[1]]),
                            smt.And(stable[preds[0]], smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(True))),
                            smt.And(stable[preds[1]], smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(True))))
            case defines.MUX_TYPE:

                preds = [cell.select, cell.mux_ins[0]]
                not_selector = smt.Not(curr_pvars[preds[0]][w])
                v_and1 = smt.And(not_selector, curr_pvars[preds[1]][w])
                r_and1 = smt.Or(smt.And(stable[preds[0]], stable[preds[1]]),
                            smt.And(stable[preds[0]], smt.Equals(not_selector, smt.BoolVal(False))),
                            smt.And(stable[preds[1]], smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(False))))

                preds = [cell.select, cell.mux_ins[1]]
                v_and2 = smt.And(curr_pvars[preds[0]][w], curr_pvars[preds[1]][w])
                r_and2 = smt.Or(smt.And(stable[preds[0]], stable[preds[1]]),
                            smt.And(stable[preds[0]], smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(False))),
                            smt.And(stable[preds[1]], smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(False))))
                
                preds = ["a", "b"]
                curr_pvars = {"a": [v_and1], "b": [v_and2]}
                stable = {"a": r_and1, "b": r_and2}
                r = smt.Or(smt.And(stable[preds[0]], stable[preds[1]]),
                            smt.And(stable[preds[0]], smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(True))),
                            smt.And(stable[preds[1]], smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(True))))

                #r = smt.Or(smt.And(stable[cell.select], stable[cell.mux_ins[0]], smt.Equals(curr_pvars[cell.select][w], smt.BoolVal(False))),
                #            smt.And(stable[cell.select], stable[cell.mux_ins[1]], smt.Equals(curr_pvars[cell.select][w], smt.BoolVal(True))),
                #            smt.And(stable[cell.select], stable[cell.mux_ins[0]], stable[cell.mux_ins[1]]))
            case _ if cell.type in REGPORT_TYPES:
                r = smt.And(smt.Equals(curr_pvars[node_id][0], curr_pvars[node_id][1]),
                            smt.Equals(curr_pvars[node_id][0], prev_pvars[node_id][0]),
                            smt.Equals(curr_pvars[node_id][0], prev_pvars[node_id][1]))
            case _:
                raise Exception(f'gate type (type={cell.type}) not implemented')
        return r

    def __gate_delta(self, cell, node_id, curr_pvars, prev_pvars, stable, preds, delta):
        # only used for stable values -> other context can be ignored
        w = 0
        match cell.type:
            case defines.CONST_TYPE:
                a = smt.BoolVal(False)
            case defines.NOT_TYPE:
                a = delta[preds[0]]
            case defines.XOR_TYPE | defines.XNOR_TYPE:
                a = smt.Or(delta[preds[0]], delta[preds[1]])
            case defines.AND_TYPE:
                a = smt.Or(smt.And(delta[preds[0]], delta[preds[1]]), # clause can be omitted
                            smt.And(delta[preds[0]], smt.Or(smt.Not(stable[preds[1]]), smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(True)))),
                            smt.And(delta[preds[1]], smt.Or(smt.Not(stable[preds[0]]), smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(True)))))
            case defines.OR_TYPE:
                a = smt.Or(smt.And(delta[preds[0]], delta[preds[1]]), # clause can be omitted
                            smt.And(delta[preds[0]], smt.Or(smt.Not(stable[preds[1]]), smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(False)))),
                            smt.And(delta[preds[1]], smt.Or(smt.Not(stable[preds[0]]), smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(False)))))
            case defines.MUX_TYPE:
                # TODO check this
                # d =  delta[s] & delta[t] & delta[e]
                #   or (not stable[s] or val[s] = 1) and delta[t]
                #   or (not stable[s] or val[s] = 0) and delta[e]
                #   or (val[e] != val[t] or not stable[t] or not stable[e]) and delta[s]
                
                
                #a = smt.Or(smt.And(stable[cell.mux_ins[0]],
                #                   smt.Or(smt.Not(stable[cell.select]), smt.Equals(curr_pvars[cell.select][w], smt.BoolVal(False)))),
                #           smt.And(stable[cell.mux_ins[1]],
                #                   smt.Or(smt.Not(stable[cell.select]), smt.Equals(curr_pvars[cell.select][w], smt.BoolVal(True)))),
                #           smt.And(stable[cell.select], smt.Or(smt.Not(stable[cell.mux_ins[0]]),
                #                                           smt.Not(stable[cell.mux_ins[1]]),
                #                                           smt.Equals(curr_pvars[cell.mux_ins[0]][w], smt.BoolVal(True)),
                #                                           smt.Equals(curr_pvars[cell.mux_ins[1]][w], smt.BoolVal(True)))))

                preds = [cell.select, cell.mux_ins[0]]
                not_selector = smt.Not(curr_pvars[preds[0]][w])
                v_and1 = smt.And(not_selector, curr_pvars[preds[1]][w])
                r_and1 = smt.Or(smt.And(stable[preds[0]], stable[preds[1]]),
                            smt.And(stable[preds[0]], smt.Equals(not_selector, smt.BoolVal(False))),
                            smt.And(stable[preds[1]], smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(False))))
                a_and1 = smt.Or(smt.And(delta[preds[0]], delta[preds[1]]), # clause can be omitted
                            smt.And(delta[preds[0]], smt.Or(smt.Not(stable[preds[1]]), smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(True)))),
                            smt.And(delta[preds[1]], smt.Or(smt.Not(stable[preds[0]]), smt.Equals(not_selector, smt.BoolVal(True)))))

                preds = [cell.select, cell.mux_ins[1]]
                v_and2 = smt.And(curr_pvars[preds[0]][w], curr_pvars[preds[1]][w])
                r_and2 = smt.Or(smt.And(stable[preds[0]], stable[preds[1]]),
                            smt.And(stable[preds[0]], smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(False))),
                            smt.And(stable[preds[1]], smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(False))))
                a_and2 = smt.Or(smt.And(delta[preds[0]], delta[preds[1]]), # clause can be omitted
                            smt.And(delta[preds[0]], smt.Or(smt.Not(stable[preds[1]]), smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(True)))),
                            smt.And(delta[preds[1]], smt.Or(smt.Not(stable[preds[0]]), smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(True)))))
                
                preds = ["a", "b"]
                curr_pvars = {"a": [v_and1], "b": [v_and2]}
                stable = {"a": r_and1, "b": r_and2}
                delta = {"a": a_and1, "b": a_and2}
                a = smt.Or(smt.And(delta[preds[0]], delta[preds[1]]), # clause can be omitted
                            smt.And(delta[preds[0]], smt.Or(smt.Not(stable[preds[1]]), smt.Equals(curr_pvars[preds[1]][w], smt.BoolVal(False)))),
                            smt.And(delta[preds[1]], smt.Or(smt.Not(stable[preds[0]]), smt.Equals(curr_pvars[preds[0]][w], smt.BoolVal(False)))))

                #a = smt.Or(
                #    smt.And(delta[cell.select], delta[cell.mux_ins[0]], delta[cell.mux_ins[1]]),
                #    smt.And(delta[cell.mux_ins[1]], smt.Or(smt.Not(stable[cell.select]),
                #                                            smt.Equals(curr_pvars[cell.select][w], smt.BoolVal(True)))),
                #    smt.And(delta[cell.mux_ins[0]], smt.Or(smt.Not(stable[cell.select]),
                #                                            smt.Equals(curr_pvars[cell.select][w], smt.BoolVal(False)))),
                #    smt.And(delta[cell.select],     smt.Or(smt.Not(stable[cell.mux_ins[0]]),
                #                                            smt.Not(stable[cell.mux_ins[1]]),
                #                                            smt.Xor(curr_pvars[cell.mux_ins[0]][w], curr_pvars[cell.mux_ins[1]][w])))
                #)
            case _ if cell.type in REGPORT_TYPES:
                a = smt.Or(smt.Xor(curr_pvars[node_id][0], curr_pvars[node_id][1]),
                            smt.Xor(prev_pvars[node_id][0], prev_pvars[node_id][1]) # TODO redo
                )
            case _:
                raise Exception(f'gate type (type={cell.type}) not implemented')
        return a
                
    def __build_glitch_smt(self, cycle, forward_activations):
        prev_pvars = self.node_pvars[cycle-1]
        curr_pvars = self.node_pvars[cycle]
        stable = self.glitch_gate_stability[cycle]
        delta = self.glitch_gate_delta[cycle]

        for node_id in self.circuit.nodes:
            cell = self.circuit.cells[node_id]
            preds = tuple(self.circuit.predecessors(node_id))
            stable[node_id] = self.__gate_stability(cell, node_id, curr_pvars, prev_pvars, stable, preds)
            delta[node_id] = self.__gate_delta(cell, node_id, curr_pvars, prev_pvars, stable, preds, delta)

    def __assert_constraints(self):
        first_cycle = min(self.node_pvars.keys())
        last_cycle = max(self.node_pvars.keys())

        for cycle in range(first_cycle, last_cycle + 1):
            curr_pvars = self.node_pvars[cycle]
            for sig_constr in self.cfg.hw_one_hots:
                if not sig_constr.in_range(cycle, last_cycle): continue
                name = sig_constr.args[0]
                bit_ids = self.cfg.hw_name_ids[name]
                pv_bits = [curr_pvars[x] for x in bit_ids]
                for pv in pv_bits: assert(all(map(lambda p: p.eq(pv[0]), pv)))
                oh_bits = [pv[0] for pv in pv_bits]
                if self.debug:
                    logging.debug(f"adding one-hot constraint for {name} in {cycle}")
                self.solver.add(smt.AtMost(*oh_bits, 1))  # at most one

            for name in self.cfg.hw_consts:
                for sig_constr in self.cfg.hw_consts[name]:
                    if not sig_constr.in_range(cycle, last_cycle): continue
                    bits = self.cfg.hw_name_ids[name]
                    type_ = self.circuit.cells[bits[0]].type
                    if cycle > 0 and type_ != PORT_TYPE:
                        logging.warning(f"WARNING: Asserting non-port {name} in cycle {cycle}")
                    if type_ not in REGPORT_TYPES:
                        logging.warning(f"WARNING: Asserting non-regport {name} in cycle {cycle}")

                    pv_bits = {x:curr_pvars[y] for x,y in enumerate(bits)}
                    const = sig_constr.args
                    if self.debug:
                        logging.debug(f"adding const constraint for {name}={const} in {cycle}")
                    for i in range(len(const)):
                        pv = pv_bits[i]
                        # assert(all(map(lambda x: x.eq(pv[0]), pv)))
                        for p in pv: self.solver.add(smt.Equals(p, smt.BoolVal(const[i])))

            # build all equivalences as required
            for sig_constr in self.cfg.hw_equivs:
                if not sig_constr.in_range(cycle, last_cycle): continue
                name1, name2 = sig_constr.args
                bits1 = self.cfg.hw_name_ids[name1]
                bits2 = self.cfg.hw_name_ids[name2]
                type1 = self.circuit.cells[bits1[0]].type
                type2 = self.circuit.cells[bits2[0]].type
                warning = type1 not in REGPORT_TYPES or type2 not in REGPORT_TYPES
                if warning:
                    logging.warning(f"Asserting equivalence of non-regport {name1} and {name2} in cycle {cycle}")
                elif self.debug:
                    logging.debug(f"Asserting equivalence of non-regport {name1} and {name2} in cycle {cycle}")

                pv_bits1 = {x: curr_pvars[y] for x, y in enumerate(bits1)}
                pv_bits2 = {x: curr_pvars[y] for x, y in enumerate(bits2)}
                for pv1, pv2 in zip(pv_bits1, pv_bits2):
                    for p1, p2 in zip(pv_bits1[pv1], pv_bits2[pv2]):
                        self.solver.add(smt.Equals(p1, p2))

            # special handling for start cycle:
            #     True -> (ack = 0 and rdata = 0)
            # special handling for pre cycle:
            #     !load_instr -> (ack = 0 and rdata = 0)
            if cycle <= 0:
                for signal in ("rdata", "ack"):
                    if signal not in self.cfg.state_map_memory: continue
                    hw_name = self.cfg.state_map_memory[signal][0]
                    signal_pvars = [curr_pvars[x] for x in self.cfg.hw_name_ids[hw_name]]
                    impls = [smt.BoolVal(True) for _ in range(NUM_CONTEXTS)]
                    if cycle == -1:
                        opcode_pvs = [curr_pvars[x] for x in self.cfg.hw_name_ids[self.cfg.hw_opcode_name][0:7]]
                        load_val = [smt.BoolVal(True) for _ in range(2)] + [smt.BoolVal(False) for _ in range(5)]
                        for domain_idx in range(NUM_CONTEXTS):
                            impls[domain_idx] = smt.Or(*[smt.Not(smt.Equals(load_val[xi], opcode_pvs[xi][domain_idx])) for xi in range(len(load_val))])
                    for pvs in signal_pvars:
                        for imp, val in zip(impls, pvs):
                            # logging.info(f"adding constraint {imp} -> ({val} == 0)")
                            self.solver.add(smt.Implies(imp, smt.Not(val)))

            if cycle >= 0 and cycle < last_cycle:
                next_pvars = self.node_pvars[cycle + 1]
                if any(map(lambda x: x not in self.cfg.state_map_memory, ("req", "we"))):
                    continue
                req_name = self.cfg.state_map_memory["req"][0]
                req_bits = self.cfg.hw_name_ids[req_name]
                we_name = self.cfg.state_map_memory["we"][0]
                we_bits = self.cfg.hw_name_ids[we_name]
                req_conds = [curr_pvars[req_bits[0]][dom] for dom in range(NUM_CONTEXTS)]
                read_conds = [smt.Not(curr_pvars[we_bits[0]][dom]) for dom in range(NUM_CONTEXTS)]

                # both grant and acknowledge need to go high
                for signal in ("gnt", "ack"):
                    if signal in self.cfg.state_map_memory:
                        sig_name = self.cfg.state_map_memory[signal][0]
                        if self.debug: logging.debug(f"adding memory constraint {req_name}@{cycle} <-> {sig_name}@{cycle + 1}")

                        sig_bits = self.cfg.hw_name_ids[sig_name]
                        sig_conseq = [smt.Equals(next_pvars[sig_bits[0]][dom], smt.BoolVal(True)) for dom in range(NUM_CONTEXTS)]
                        for dom_idx in range(NUM_CONTEXTS):
                            self.solver.add(smt.Equals(req_conds[dom_idx], sig_conseq[dom_idx]))

                if "rdata" in self.cfg.state_map_memory:
                    hw_name, sw_name = self.cfg.state_map_memory["rdata"]
                    if self.debug: logging.debug(f"adding memory constraint {req_name}@{cycle} -> {hw_name}@{cycle + 1} == {sw_name}")
                    k_ty = self.cfg.ctrct_state_var[sw_name]
                    sw_smt_vars = self.pvars_contract(sw_name, k_ty)
                    hw_bits = self.cfg.hw_name_ids[hw_name]
                    assert (k_ty.length == len(hw_bits))
                    hw_pos_pvars = [next_pvars[bit] for bit in hw_bits]
                    # for each bit in a register
                    for dom_idx in range(NUM_CONTEXTS):
                        read_req = smt.And(req_conds[dom_idx], read_conds[dom_idx])
                        for bit_pos in range(0, k_ty.length):
                            conseq = smt.Equals(hw_pos_pvars[bit_pos][dom_idx], smt_extract_bool(sw_smt_vars[dom_idx], bit_pos))
                            # self.solver.add(smt.Implies(req_conds[dom_idx], conseq))
                            # if there is a request, then make them equal otherwise zero
                            self.solver.add(smt.If(read_req, conseq, smt.Not(hw_pos_pvars[bit_pos][dom_idx])))
        if self.sanitycheck:
            assert(self.solver.check() == smt.sat), "The given configuration constraints are not satisfiable"
        pass

    def pvars_contract(self, k, k_ty):
        # construct the smt names of the corresponding contract registers
        sw_smt_vars = []
        for suff in SUFFIXES:
            ctx_nm = k + suff
            if not(ctx_nm in self.ctrct_substitutes):
                logging.error(f"{ctx_nm} not defined in contract .map")
                sys.exit(EXIT_CODE_INPUT_ERR)
            else:
                if k_ty.z3_ty == Z3_TYPE_BITVEC:
                    sw_smt_vars.append(smt.BitVec(self.ctrct_substitutes[ctx_nm], k_ty.length, exists=True))
                elif k_ty.z3_ty == Z3_TYPE_BOOL:
                    sw_smt_vars.append(smt.Bool(self.ctrct_substitutes[ctx_nm], exists=True))
                else:
                    assert False
        return sw_smt_vars

    def make_precycle_implications(self):
        # At this point, in the propagation part, we have already proven these statements:
        # [eq(creg[1]) and eq(creg[1]) and ... => eq(cell)]
        # We did this by asking a solver if it can satisfy the following and getting UNSAT:
        # [eq(creg[1]) and eq(creg[1]) and ... and not eq(cell)]
        # because [X => Y] is the same as [not Y => not X], we can add the statements
        # [diff(cell) => diff(creg[1]) or diff(creg[2]) or ...]
        assert self.mode in (TRANSIENT, GLITCH)
        diff_cache = {}
        pre_cycle_pvars = self.node_pvars[-1]
        for cell_id, cregs in self.cfg.state_map_cellsw.items():
            cell_id = int(cell_id)
            cell = self.circuit.cells[cell_id]
            
            if self.mode == GLITCH:
                assert cell.type in REGPORT_TYPES, f'implication in config is not a register or port (cell id = {cell_id})'
            
            # TODO add implications also for other cycles?
            if len(cregs) == 1 and cregs[0] == "public":
                #if cell.type in REGISTER_TYPES:
                #    # TODO clean up
                #    #regtype = cell.type
                #    #logging.info(f'asserting equivalence of {cell_id} in pre-cycle ({regtype})')
                pvars = pre_cycle_pvars[cell_id]
                self.solver.add(smt.Equals(pvars[0], pvars[1]))
                continue
            
            # FIXME, turn these into ASSERTIONS
            #if cell.type in REGISTER_TYPES: continue
            if len(cregs) == 0: continue
            #if cregs[0] == "public": continue

            # extract information if cell is different in the initial cycle [the alignment cycle]
            pvars = pre_cycle_pvars[cell_id]
            diff_cell = smt.Xor(pvars[0], pvars[1])

            # extract formula to tell if one of the cregs is different
            diff_ctrct = []
            for creg in cregs:
                creg_ty = self.cfg.ctrct_state_var[creg]
                creg_pvars = self.pvars_contract(creg, creg_ty)
                assert (creg_ty.z3_ty == Z3_TYPE_BITVEC)
                if creg in diff_cache:
                    diff_creg = diff_cache[creg]
                else:
                    diff_creg_lst = []
                    for bit_pos in range(creg_ty.length):
                        diff_creg_lst.append(smt.Xor(smt_extract_bool(creg_pvars[0], bit_pos),
                                                    smt_extract_bool(creg_pvars[1], bit_pos)))
                    diff_creg = smt.Or(*diff_creg_lst)
                    diff_cache[creg] = diff_creg
                diff_ctrct.append(diff_creg)

            self.solver.add(smt.Implies(diff_cell, smt.Or(*diff_ctrct)))
        if self.sanitycheck:
            assert (self.solver.check() == smt.sat), "internal bug during alignment of contract and HW circuit"
            
    def assert_precycle_equality(self, creg, hwreg):
        assert self.mode in (TRANSIENT, GLITCH)
        pre_cycle_pvars = self.node_pvars[-1]
        nids = self.cfg.hw_name_ids[hwreg]

        for world in (0, 1):
            hwreg_pvars = [pre_cycle_pvars[nid][world] for nid in nids]
            k_ty = self.cfg.ctrct_state_var[creg]
            assert(len(hwreg_pvars) == k_ty.length)
            creg_bvvar = self.pvars_contract(f"{creg}", k_ty)[world]
            creg_pvars = [smt_extract_bool(creg_bvvar, i) for i in range(k_ty.length)]

            cond = smt.And(*[smt.Equals(a, b) for a, b in zip(hwreg_pvars, creg_pvars)])
            self.solver.add(cond)

    def restraing_precycle_pc(self):
        hwreg = "u_ibex_core.pc_id"
        assert self.mode in (TRANSIENT, GLITCH)
        pre_cycle_pvars = self.node_pvars[-1]
        nids = self.cfg.hw_name_ids[hwreg]

        for world in (0, 1):
            hwreg_pvars = [pre_cycle_pvars[nid][world] for nid in nids]

            cond = smt.And(smt.Equals(hwreg_pvars[0], smt.BoolVal(False)), smt.Equals(hwreg_pvars[1], smt.BoolVal(False)))
            self.solver.add(cond)
            
    def make_precycle_equalities(self):
        self.restraing_precycle_pc() # TODO check
        last_cycle = max(self.node_pvars.keys())
        for ct_var, mappings in self.cfg.hwct_equivs.items():
            for cycle_annot, hw_vars in mappings.items():
                for hw_var in hw_vars:
                    cycle = SignalConstraint.decode(*cycle_annot, last_cycle)
                    if not (cycle == last_cycle):
                        logging.error('cycle annotation not within expected range')
                        sys.exit(-1)
                    self.assert_precycle_equality(ct_var, hw_var)
        
    def load_contract_files(self, suffix):
        # load the contract encoded as smt
        fn = self.path_contract_base + suffix
        fnmap = fn + ".map"
        if not (path.isfile(fn) and path.isfile(fnmap)):
            logging.error(f"file {fn} or {fnmap} don't exist)")
            sys.exit(EXIT_CODE_INPUT_ERR)

        self.solver.from_file(fn)
        self.ctrct_substitutes = parse_ctrct_name_map(fnmap)

        ASSERT_PREF = "assert_indep_"
        LEAK_PREF = ASSERT_PREF + "leak"
        for creg in self.ctrct_substitutes.keys():
            if not creg.startswith(LEAK_PREF): continue
            num = int(creg[len(LEAK_PREF):].split("_")[0])
            if num not in self.leak_statements:
                self.leak_statements[num] = []
            self.leak_statements[num].append(creg[len(ASSERT_PREF):])
        logging.info(f'leaks loaded: {len(self.leak_statements)}')
        return

    def align_contract(self):
        initial_pvars = self.node_pvars[0]
        # use the mapping to define which hw/sw register/state pairs are asserted equal
        tasks = []
        tasks += [("equal", sw_name, hw_name) for (sw_name, hw_name) in self.cfg.state_map_swhw.items()]
        tasks += [("equal", sw_name, hw_name) for (hw_name, sw_name) in self.cfg.state_imp_hwsw.items()]

        for task, sw_name, hw_name in tasks:
            logging.info(f"aligning {task} {sw_name} {hw_name} in cycle {0}")
            # grab the SMT type of k from its definition in the config file
            k_ty = self.cfg.ctrct_state_var[sw_name]
            # look-up the variables corresponding to each bit of hw-register v
            nids = self.cfg.hw_name_ids[hw_name]
            hw_pos_pvars = {pos: initial_pvars[nid] for pos, nid in enumerate(nids)}
            assert(len(hw_pos_pvars) <= k_ty.length), f"{sw_name} (length {k_ty.length}) has different length than {hw_name} (length {len(hw_pos_pvars)})"
            sw_smt_vars = self.pvars_contract(sw_name, k_ty)

            if task == "equal":
                # for each bit in a register
                for bit_pos in range(0, k_ty.length):
                    if bit_pos not in hw_pos_pvars.keys():
                        # FIXME @VEDAD
                        # ignore those bits that are not defined by HW
                        continue
                    # get the four-tuple corresponding to each context
                    hw_smt_vars = hw_pos_pvars[bit_pos]
                    assert len(hw_smt_vars) == NUM_CONTEXTS

                    # add an equality constraint for each of the four contexts
                    for domain_idx in range(NUM_CONTEXTS):
                        assert(k_ty.z3_ty == Z3_TYPE_BITVEC)
                        self.solver.add(smt.Equals(hw_smt_vars[domain_idx], smt_extract_bool(sw_smt_vars[domain_idx], bit_pos)))
            if task == "implies":
                for domain_doubles in range(0, NUM_CONTEXTS, 2):
                    hw_deps = [smt.Xor(pvars[domain_doubles], pvars[domain_doubles + 1]) for pvars in hw_pos_pvars.values()]
                    hw_dep = smt.Or(*hw_deps)
                    sw_deps = [smt.Xor(smt_extract_bool(sw_smt_vars[domain_doubles], bit_pos),
                                      smt_extract_bool(sw_smt_vars[domain_doubles + 1], bit_pos))
                               for bit_pos in range(k_ty.length)]
                    sw_dep = smt.Or(*sw_deps)
                    self.solver.add(smt.Implies(hw_dep, sw_dep))


        # create a constraint that the to-be-executed opcodes in hw and contract are the same
        sw_opc_len = self.cfg.ctrct_opcode_ty.length
        sw_opc_name = self.ctrct_substitutes[self.cfg.ctrct_opcode + "1"]
        sw_opc = smt.BitVec(sw_opc_name, sw_opc_len, exists=True)
        opcode_ids = self.cfg.hw_name_ids[self.cfg.hw_opcode_name]
        assert(len(opcode_ids) == sw_opc_len)
        # FIXME: Hardcoded equivalence in first cycle
        for i in range(sw_opc_len):
            assert self.cfg.ctrct_opcode_ty.z3_ty == Z3_TYPE_BITVEC
            self.solver.add(smt.Equals(smt_extract_bool(sw_opc, i), initial_pvars[opcode_ids[i]][0]))

        if self.sanitycheck:
            assert(self.solver.check() == smt.sat), "internal bug during alignment of contract and HW circuit"

        if self.storeartifacts:
            with open(self.path_contract_base + "_combined_hwsw.smt2", "w") as f:
                f.write(self.solver.to_smt2())
        pass

    def get_bit_diff_str(self, pvars, model):
        val_0 = model.eval(pvars[0], True).sexpr()
        val_1 = model.eval(pvars[1], True).sexpr()

        diff = val_0 != val_1
        if diff: return "X"
        return str((val_0 in ["true", "1"]) & 1)

    def get_register_diff_str(self, curr_pvars, ids, model):
        bits = [self.get_bit_diff_str(curr_pvars[bit_id], model) for bit_id in ids][::-1]
        return "".join(bits)

    def print_finding(self, description, uid=None):
        self.num_leaks -= 1

        fn = None
        if self.counterexamplecounter >= self.max_cexes:
            self.counterexamplecounter += 1
            logging.info(f"suppressing counterexample #{self.counterexamplecounter}: {description}")
            return
        elif self.output is None:
            # redirect to logfile
            def redirect(msg): logging.info(msg)
        else:
            # write to separate file
            self.counterexamplecounter += 1
            suff = f"{self.counterexamplecounter}" if uid == None else f"{uid}"
            suff = suff[:200] # do not exceed max file name
            filename = f"{self.output}-counterexample-{suff}.txt"
            fn = open(filename, "w")
            logging.info(f"redirecting counterexample {description} to {filename}")
            fn.write(description + "\n")
            def redirect(msg): fn.write(msg + "\n")

        model = self.solver.model()
        printed = set()
        first_cycle = min(self.node_pvars)
        last_cycle = max(self.node_pvars)
        for cycle in range(first_cycle, last_cycle + 1):
            redirect(f"\n### beginning of cycle {cycle} ###\n")
            curr_pvars = self.node_pvars[cycle]
            # extract the current opcode
            opcode_ids = self.cfg.hw_name_ids[self.cfg.hw_opcode_name]
            opcode = self.get_register_diff_str(curr_pvars, opcode_ids, model)

            disass_text = ""
            if self.disassembler is not None:
                disass_text = self.disassembler.disassemble(int(opcode, 2))
                explain_text = self.disassembler.explain([int(opcode, 2)])
                redirect(f"{explain_text}")
            redirect(f"opcode = 0b{opcode} {disass_text}")

            # print all values defined in mappings first (grouping register bits into one register)
            redirect("\n# hardware variables in contract")
            pairs = []
            pairs += [(creg, hwreg) for creg, hwreg in self.cfg.state_map_swhw.items()]
            pairs += [(creg, hwreg) for hwreg, creg in self.cfg.state_imp_hwsw.items()]
            for creg, hwreg in pairs:
                creg_size = self.cfg.ctrct_state_var[creg].length
                hwreg_bits = self.cfg.hw_name_ids[hwreg]
                if creg_size != len(hwreg_bits): continue # FIXME print difficult registers

                # get the value of each bit
                regval = self.get_register_diff_str(curr_pvars, hwreg_bits, model)
                redirect(f"{creg} ({hwreg}) = 0b{regval}")
                printed.update(hwreg_bits)

            # print the remaining definitions of the hardware
            redirect("\n# hardware vars not in contract")
            for hwreg in self.cfg.hw_state_var:
                # skip cells which have already been printed
                hwreg_bits = self.cfg.hw_name_ids[hwreg]
                if all(map(lambda x: x in printed, hwreg_bits)): continue
                regval = self.get_register_diff_str(curr_pvars, hwreg_bits, model)
                redirect(f"{hwreg} = 0b{regval}")

            # print values defined as constants
            redirect("\n# hardware constants with known value")
            for const_name in self.cfg.hw_consts:
                for sig_constr in self.cfg.hw_consts[const_name]:
                    if not sig_constr.in_range(cycle, last_cycle): continue
                    hw_bits = self.cfg.hw_name_ids[const_name]
                    regval = "".join([self.get_bit_diff_str(curr_pvars[bit_id], model) for bit_id in hw_bits][::-1])
                    expected = "".join([str(x & 1) for x in sig_constr.args][::-1])
                    redirect(f"{const_name} = 0b{regval} (should be: 0b{expected})")

            # print values defined as one-hot
            redirect("\n# hardware constants one-hot")
            for sig_constr in self.cfg.hw_one_hots:
                ohn = sig_constr.args[0]
                hw_bits = self.cfg.hw_name_ids[ohn]
                regval = "".join([self.get_bit_diff_str(curr_pvars[bit_id], model) for bit_id in hw_bits][::-1])
                redirect(f"{ohn} = 0b{regval} (should be one-hot)")

            # print values defined as public
            redirect("\n# other hardware public constants")
            for const in self.cfg.hw_state_pub:
                if (const == self.cfg.hw_opcode_name or
                    const in self.cfg.hw_consts or
                    const in self.cfg.hw_one_hots or
                    const in self.cfg.hw_state_debug): continue
                hw_bits = self.cfg.hw_name_ids[const]
                regval = "".join([self.get_bit_diff_str(curr_pvars[bit_id], model) for bit_id in hw_bits][::-1])
                redirect(f"{const} = 0b{regval}")

            redirect("\n# debug hardware constants")
            for const in self.cfg.hw_state_debug:
                hw_bits = self.cfg.hw_name_ids[const]
                regval = "".join([self.get_bit_diff_str(curr_pvars[bit_id], model) for bit_id in hw_bits][::-1])
                redirect(f"{const} = 0b{regval}")
                
                # TODO beautify
                #print('lenlen', len(self.solver.expr_cache))
                
                if cycle == -1: continue
                #print(self.glitch_gate_stability.keys())
                #print('dbg234', self.glitch_gate_stability[cycle][hw_bits[0]])
                
                #for bit_id in hw_bits:
                #    val = model.eval(self.glitch_gate_stability[cycle][bit_id], True).sexpr()
                #    print(bit_id, val)
                #    
                #continue
                
                regstability = "".join([model.eval(self.glitch_gate_stability[cycle][bit_id], True).sexpr() for bit_id in hw_bits][::-1])
                redirect(f"{const} [stabi] = 0b{regstability}")
                
                regdelta = "".join([model.eval(self.glitch_gate_delta[cycle][bit_id], True).sexpr() for bit_id in hw_bits][::-1])
                redirect(f"{const} [delta] = 0b{regdelta}")

            redirect("\n# other contract variables (initial values in contract)")
            for crego in self.cfg.ctrct_state_var:
                # rename the var
                creg = crego
                creg_ty = self.cfg.ctrct_state_var[crego]
                assert(creg_ty.z3_ty == Z3_TYPE_BITVEC)

                pvals = self.pvars_contract(creg, creg_ty)
                pvars_i = [[smt_extract_bool(x, i) for x in pvals] for i in range(creg_ty.length)]
                if len(pvars_i) and len(pvals) == 2:
                    pvars_i = [p + p for p in pvars_i]
                regvals = [self.get_bit_diff_str(pvars, model) for pvars in pvars_i]
                regstr = "".join(regvals[::-1])
                redirect(f"{crego} = 0b{regstr}")

            redirect("\n# other contract variables (final values in contract)")
            for crego in self.cfg.ctrct_state_var:
                # rename the var
                creg = crego + "_final"
                creg_ty = self.cfg.ctrct_state_var[crego]
                assert(creg_ty.z3_ty == Z3_TYPE_BITVEC)

                pvals = self.pvars_contract(creg, creg_ty)
                pvars_i = [[smt_extract_bool(x, i) for x in pvals] for i in range(creg_ty.length)]
                if len(pvars_i) and len(pvals) == 2:
                    pvars_i = [p + p for p in pvars_i]
                regvals = [self.get_bit_diff_str(pvars, model) for pvars in pvars_i]
                regstr = "".join(regvals[::-1])
                redirect(f"{crego} = 0b{regstr}")

            for leak_idx in self.leak_statements:
                name = f"leak{leak_idx}_is_active_final"
                pvars = [smt.Bool(self.ctrct_substitutes[name + suff], exists=True) for suff in SUFFIXES]
                pstr = self.get_bit_diff_str(pvars, model)
                redirect(f"{name} = 0b{pstr}")
                if pstr == '1':
                    leak_exprs = self.leak_statements[leak_idx]
                    for crego in sorted(leak_exprs, key=lambda x: int(x.split('_e')[-1])):
                        creg = crego + "_final"
                        creg_ty = CfgZ3Typ("BitVec 32")

                        pvals = self.pvars_contract(creg, creg_ty)
                        pvars_i = [[smt_extract_bool(x, i) for x in pvals] for i in range(creg_ty.length)]
                        if len(pvars_i) and len(pvals) == 2:
                            pvars_i = [p + p for p in pvars_i]
                        regvals = [self.get_bit_diff_str(pvars, model) for pvars in pvars_i]
                        regstr = "".join(regvals[::-1])
                        redirect(f"{crego} = 0b{regstr}")
                    
                    

            redirect(f"\n### end of cycle {cycle} ###\n")

        if self.output is not None and fn is not None:
            fn.close()

        if self.storeartifacts:
            with open(f"{self.output}-finding.model", "w+") as f:
                f.write(str(model))

    # Returns 10 if any nid has a difference with the given constraints (sat)
    # Returns 20 if no nid cannot be different given constraints (unsat)
    def __check_diff_nids_against_cregs(self, nids, cycles="", cregs="", debug=False, dryrun=False, actives="", assumptions=[], with_glitches=True):
        #assert (self.solver.num_scopes() == 1) # TODO reenable
        #self.solver.push()

        # convert params
        assert(type(nids) == str)
        nids = list(map(int, nids.split()))
        assert(len(nids) != 0)

        assert(type(cycles) == str)
        cycles = list(map(int, cycles.split()))
        assert(len(cycles) != 0)

        assert(type(cregs) == str)
        assert(type(actives) == str)
        if type(debug) == str: debug = (debug == "True")
        if type(dryrun) == str: dryrun = (dryrun == "True")
        
        assumes = []
        assumes += assumptions

        # These additional contract conditions will be asserted as well
        indep_names = [self.ctrct_substitutes[f"assert_indep_{creg}"] for creg in cregs.split()]
        cond_bools = [(smt.Bool(cond, exists=True)) for cond in indep_names if cond != ""]
        if len(cond_bools) != 0:
            #self.solver.add(smt.And(*cond_bools))
            assumes.append(smt.And(*cond_bools))

        # These are additional leak activity constraints
        active_bools = []
        for act in actives.split():
            neg = False
            if act.startswith("-"):
                act = act[1:]
                neg = True
            act_bools = [smt.Bool(self.ctrct_substitutes[f"{act}_is_active_final{suff}"], exists=True) for suff in SUFFIXES]
            if neg: act_bools = [smt.Not(b) for b in act_bools]
            active_bools += act_bools

        # print(active_bools)
        if len(active_bools) != 0:
            expr = smt.And(*active_bools)
            #self.solver.add(expr)
            assumes.append(expr)

        # For each cell, these constraints give their context difference
        node_diff_conds = []
        if with_glitches and self.mode == GLITCH:
            for cycle in cycles:
                #cycle_pvars = self.node_pvars[cycle]
                cycle_delta = self.glitch_gate_delta[cycle]
                for nid in nids:
                    #node_delta = cycle_delta[nid]
                    #for hwreg, delta_smt in node_delta.items():
                    #    reg_gate_list = self.glitch_registers[hwreg]
                    #    reg_pvars_diff = [smt.Xor(cycle_pvars[x][0], cycle_pvars[x][1]) for x in reg_gate_list]
                    #    reg_pvars_diff.append(smt.Xor(delta_smt[0], delta_smt[1]))
                    #    node_diff_conds.append(smt.Or(*reg_pvars_diff)) # TODO check and optimize
                    #print(f'=====================\n{cycle_delta[nid]}\n=====================\n')
                    node_diff_conds.append(cycle_delta[nid])
        else:
            for cycle in cycles:
                cycle_pvars = self.node_pvars[cycle]
                pvars_list = [cycle_pvars[nid] for nid in nids]
                for pvars in pvars_list: node_diff_conds.append(smt.Xor(pvars[0], pvars[1]))
        #self.solver.add(smt.Or(*node_diff_conds))
        assumes.append(smt.Or(*node_diff_conds))

        # Check whether there are configurations so that:
        # * any cell in the bunch is different in contexts
        # * all conditions from outside are upheld
        r = self.solver.check(assumes, modelgen=debug) if (not dryrun) else smt.unsat
        assert (r != smt.unknown)
        ret = 10 if (r == smt.sat) else 20
        if debug and r == smt.sat:
            cells = [self.circuit.cells[nid] for nid in nids]
            logging.debug(f"cells {nids} can become dependent: {cells}")
            suf = "_".join(map(str, nids)) + "_".join(cregs.split()) + "_".join(actives.split())
            vcycles = "_".join(map(str, cycles))
            self.print_finding(f"__check_diff_nids_against_cregs in cycles {cycles} for nodes {nids} against {cregs} ({cells})", uid=f"{suf}-{vcycles}")
        #self.solver.pop()
        return ret

    def __run_multi(self, fun, queue, do_all=True, polarity=True):
        if self.sequential or len(queue) <= 1:
            return parallel.run_multi_sequential(fun, queue, do_all, polarity)
        else:
            return parallel.run_multi_parallel(fun, queue, do_all, polarity, worker_count=self.processes)
        
    def __check_prop_specified(self, considered=None):
        logging.info("checking propagation of specified mappings")
        cycles = str(max(self.node_pvars.keys()))
        # iterate over all cells
        tasks = []
        if considered == None:
            considered = set()

        # check the mapping for registers / leakagestate
        # task will be Or(dep_reg_in_bits) and indep_contract_var
        pairs = []
        if not(self.dryrun_prop_specified_lst):
            pairs += [(hwreg, creg) for (hwreg, creg) in self.cfg.state_imp_hwsw.items()]
        if not(self.dryrun_prop_specified_reg):
            pairs += [(hwreg, creg) for (creg, hwreg) in self.cfg.state_map_swhw.items()]

        for hwreg, creg in pairs:
            # get the correct ids, that is, those of predecessors
            hwreg_bits = self.cfg.hw_name_ids[hwreg]
            nids_ = [tuple(self.circuit.predecessors(reg_bit_id)) for
                     reg_bit_id in hwreg_bits]
            assert(all(map(lambda x: len(x) == 1, nids_)))
            nids = [nid_[0] for nid_ in nids_]
            if (all(n in considered for n in nids) and
                all(n in considered for n in hwreg_bits)):
                continue
            # mark them all as considered
            considered.update(hwreg_bits)
            considered.update(nids)
            tasks.append((helpers.ints_as_str(nids), cycles, creg, True, False))
            # TODO do checks for each nid individually if enough cores are available?

        # check previous propagation check results from config
        # task for public mapping will be just dep_cell
        # task for simulation mapping will be dep_cell and And(indep_simulators)
        for nid, cregs in self.cfg.state_map_cellsw.items():
            nid = int(nid)
            if nid in considered: continue
            considered.add(nid)
            if len(cregs) == 1 and cregs[0] == "public":
                tasks.append((str(nid), cycles, "", True, self.dryrun_prop_specified_pub))
            else:
                tasks.append((str(nid), cycles, " ".join(cregs), True, self.dryrun_prop_specified_map))

        # check wires/state declared as public
        # task will be just Or(dep_wire_cells)
        for hw_name in self.cfg.hw_state_pub:
            hw_bits = self.cfg.hw_name_ids[hw_name]
            considered.update(hw_bits)
            tasks.append((helpers.ints_as_str(hw_bits), cycles, "", True, self.dryrun_prop_specified_pub))

        # launch the check
        results = self.__run_multi(self.__check_diff_nids_against_cregs, tasks, do_all=True)

        # check the results
        for i in results.keys():
            # smt.unsat == 20 == False => never depending on A given the mapping constraints
            if results[i]:
                logging.debug(f"task {i} failed: smt.sat for {tasks[i]}")
            else:
                cregs = tasks[i][2].split()
                for nid in tasks[i][0].split():
                    self.__update_simulators(nid, cregs)

        if any(results.values()):
            logging.debug(f"failed in __check_prop_specified for {[tasks[i] for i in range(len(results)) if results[i]]}")
            return False, set()

        return True, considered
    
    def __check_equiv_specified_limited(self, creg, cycle, hwreg):
        last_cycle = max(self.node_pvars.keys())
        cycle_pvars = self.node_pvars[last_cycle]
        nids = self.cfg.hw_name_ids[hwreg]

        hwreg_pvars = [cycle_pvars[nid][0] for nid in nids]
        k_ty = self.cfg.ctrct_state_var[creg]
        assert(len(hwreg_pvars) == k_ty.length)
        creg_bvvar = self.pvars_contract(f"{creg}_final", k_ty)[0]
        creg_pvars = [smt_extract_bool(creg_bvvar, i) for i in range(k_ty.length)]
        logging.info(f"Checking {creg} != {hwreg}")

        cond = smt.Or(*[smt.Xor(a, b) for a, b in zip(hwreg_pvars, creg_pvars)])
        res = self.solver.check(cond)
        if res == smt.sat:
            self.print_finding("Found counterexample", "wtf-error")
            return smt.sat
        else:
            logging.info(f" Proven that {creg} == {hwreg}")
            return smt.unsat

    def __check_equiv_specified(self):
        logging.info("checking hw-contract equalities of specified mappings")
        last_cycle = max(self.node_pvars.keys())
        #cycles = str(max(self.node_pvars.keys()))
        
        tasks = []
        for ct_var, mappings in self.cfg.hwct_equivs.items():
            for cycle_annot, hw_vars in mappings.items():
                for hw_var in hw_vars:
                    cycle = SignalConstraint.decode(*cycle_annot, last_cycle)
                    if not (cycle == last_cycle):
                        logging.error('cycle annotation not within expected range')
                        return False
                    tasks.append((ct_var, cycle, hw_var))

        results = self.__run_multi(self.__check_equiv_specified_limited, tasks, do_all=True)

        for i in results.keys():
            if results[i]:
                logging.debug(f"task {i} failed: smt.sat for {tasks[i]}")

        if any(results.values()):
            logging.debug(f"failed in __check_equiv_specified for {[tasks[i] for i in range(len(results)) if results[i]]}")
            return False
        return True
        
    def __check_prop_simulatable(self, ignored):
        logging.info("checking if we can simulate everything")
        tasks = []
        all_ctrct_regs = list(self.cfg.ctrct_state_var.keys())
        ctrct_indep_str = " ".join(all_ctrct_regs)
        cycles = str(max(self.node_pvars.keys()))

        for nid in self.circuit.nodes:
            if nid in ignored: continue
            if self.circuit.cells[nid].type in REGISTER_TYPES: continue
            tasks.append((str(nid), cycles, ctrct_indep_str, True, self.dryrun_prop_simulatable))

        results = self.__run_multi(self.__check_diff_nids_against_cregs, tasks)

        counter = 0
        with open(self.output + ".missing", "w+") as f:
            f.write(f"\n// analysis results added on {datetime.now()}\n")
            # we potentially need to exclude stuff here
            for i, r in results.items():
                nid = int(tasks[i][0])
                if r:
                    counter += 1
                    cell = self.circuit.cells[int(tasks[i][0])]
                    logging.debug(f"not simulatable: {cell}")
                    f.write(f"{cell.name}: {cell}\n")
                    ignored.add(nid)
            logging.debug(f"total missing: {counter}")
            f.write(f"total missing: {counter}\n")

        return counter == 0, ignored

    def __find_prop_public(self, ignore):
        logging.info("searching for public values")
        cycles = str(max(self.node_pvars.keys()))
        tasks = [(str(nid), cycles, "", False, self.dryrun_prop_find_public)
                 for nid in self.circuit.nodes
                 if nid not in ignore]
        # tasks = tasks[:3000]
        results = self.__run_multi(self.__check_diff_nids_against_cregs, tasks)

        pub_nodes = set()

        for i, res in results.items():
            nid = int(tasks[i][0])
            if res:
                logging.debug(f"not public: {self.circuit.cells[nid]}")
            else:
                pub_nodes.add(nid)
                self.__update_simulators(str(nid), [])

        covered = set()
        cfg_path = self.output + f"-public-{len(self.node_pvars)}-cycles.cfg"

        with open(cfg_path, "w+") as f:
            f.write(f"sanity hash {self.cfg.circ_hash}\n")
            key_fun = (lambda x: (x[0].startswith("_") & 1) * 100000 - len(x[1]))
            wire_order = sorted(self.cfg.hw_name_ids.items(), key=key_fun)
            for hw_name, bits in wire_order:
                if all(map(lambda x: x in pub_nodes, bits)) and not covered.issuperset(bits):
                    #logging.debug(f"{hw_name} is public")
                    f.write(f"mapping wire {hw_name} public\n")
                    covered.update(bits)
            other_pub =  pub_nodes.difference(covered)
            for nid in other_pub:
                f.write(f"mapping cell {nid} public\n")
        return True, ignore.union(pub_nodes)
    
    def __find_initial_nodes(self, ignored_nids: set[int]):
        initial_nodes = []

        for nid in self.circuit.nodes:
            if nid in ignored_nids:
                continue
            if self.circuit.cells[nid].type in REGPORT_TYPES:
                initial_nodes.append(nid)
            elif all(pred in ignored_nids for pred in self.circuit.all_predecessors(nid)):
                initial_nodes.append(nid)
                
        assert len(set(initial_nodes)) == len(initial_nodes), 'not unique'
        return initial_nodes
        

    def __find_prop_simulators(self, ignored):
        logging.info("searching for contract regs that simulate each value")
        all_ctrct_cregs = list(self.cfg.ctrct_state_var.keys())
        simulator_group_checks = self.cfg.simulation_groups + [(x,) for x in all_ctrct_cregs]
        cycles = str(max(self.node_pvars.keys()))
        simulators = {}
        initial_nodes = []

        for nid in self.circuit.nodes:
            if nid in ignored: continue
            simulators[nid] = set(all_ctrct_cregs)
            #if cycles == '0':
            #    assert self.circuit.cells[nid].type not in REGPORT_TYPES
            if self.circuit.cells[nid].type in REGPORT_TYPES:
                initial_nodes.append(nid)
            elif all(pred in ignored for pred in self.circuit.all_predecessors(nid)):
                initial_nodes.append(nid)
        
        # statistics
        total_scheduled = 0
        skipped1 = 0
        skipped2 = 0
        skipped3 = 0

        NEW_CKECK = True
        NEW_CKECK = False
        if NEW_CKECK:
            for gidx, cregs in enumerate(simulator_group_checks):
                scheduled = set()
                done = set()
                
                def schedule(nid, sched_func, add_result=None):
                    nonlocal total_scheduled, skipped1, skipped2, skipped3
                    if nid in scheduled:
                        return
                    # delay schedule until all predecessors are done:
                    for pred in self.circuit.all_predecessors(nid):
                        if pred not in ignored and pred not in done:
                            logging.debug(f'delay schedule of {nid}: waiting for {pred}')
                            return
                    
                    scheduled.add(nid)
                    total_scheduled += 1
                    
                    conds = simulators[nid]
                    remaining = conds.difference(cregs)
                    
                    deduced = False # TODO find better option to skip process
                    
                    # skip check if a previous simgroup already removed all registers from simulators
                    if remaining == conds:
                        skipped1 += 1
                        deduced = True
                    
                    if not deduced:
                        # skip check if a previous failed simgroup would have removed a subset of the current group
                        for prev_group in simulator_group_checks[:gidx]:
                            remaining_in_group = conds.union(prev_group)
                            if len(remaining_in_group) > 0 and remaining_in_group.issubset(cregs):
                                deduced = True
                                skipped2 += 1
                                break
                        
                    if not deduced:
                        if self.circuit.cells[nid].type == PORT_TYPE:
                            pass # ports can never be deduced
                        elif cycles == '0' and self.circuit.cells[nid].type in REGPORT_TYPES:
                            pass # regports cannot be decuced in first cycle
                        else:
                            # skipt check if all predecessors can be simulated with a subset of the remaining simulators
                            logging.debug(f'deduction for {nid}: start deducing {cregs}')
                            for pred in self.circuit.all_predecessors(nid):
                                #if self.circuit.cells[pred].type in REGISTER_TYPES:
                                #    pred_simulators = regport_simulators[pred]
                                if pred in ignored:
                                    # TODO self.node_simulators is not safe here, need to copy node simulators from last round
                                    if pred in self.node_simulators and self.node_simulators[pred].issubset(remaining):
                                        logging.debug(f'deduction for {nid}: skip covered predecessor {pred} ({self.node_simulators[pred]})')
                                        continue # skip public predecessor
                                    logging.warn(f'Node {pred} has been marked as checked but is not public')
                                    break # deduction not possible
                                if not simulators[pred].issubset(remaining):
                                    logging.debug(f'deduction for {nid}: failed deduction of {cregs} because {pred} has incompatible simulator set ({simulators[pred]} - {remaining} = {simulators[pred] - remaining})')
                                    break # deduction not possible
                                logging.debug(f'deduction for {nid}: pred {pred} is accepted ({simulators[pred]})')
                            else:
                                logging.debug(f'deduction for {nid}: deduced {cregs}')
                                skipped3 += 1
                                deduced = True
                    
                    args = (str(nid), cycles, ' '.join(remaining), False, self.dryrun_prop_find_simulators, "", [], False)
                    if deduced:
                        add_result(args, smt.unsat)
                    else:
                        sched_func(args)
                
                def on_result(args, res, add_to_queue, add_result):
                    logging.debug(f'{args} -> {res}')
                    nid = int(args[0])
                    assert res in [smt.sat, smt.unsat]
                    # if still unsat, then its fine to remove it for real
                    if res == smt.unsat:
                        for creg in cregs:
                            simulators[nid].discard(creg)
                    done.add(nid)
                    for succ in self.circuit.all_successors(nid):
                        if succ not in ignored:
                            schedule(succ, add_to_queue, add_result)

                initial_queue = []
                result_backlog = []
                def add_result(args, res):
                    result_backlog.append((args, res))
                for nid in initial_nodes:
                    schedule(nid, initial_queue.append, add_result)
                while len(result_backlog) > 0:
                    args, res = result_backlog.pop(0)
                    on_result(args, res, initial_queue.append, add_result)
                
                logging.info(f"checking whether {cregs} is required for simulation ({gidx} out of {len(simulator_group_checks)})")
                parallel.run_multi_interactive(self.__check_diff_nids_against_cregs, initial_queue, on_result=on_result)
                assert simulators.keys() == scheduled, f'{sorted(simulators.keys())} != {sorted(scheduled)}' # TODO rervmoe error print
            
            total_skipped = skipped1+skipped2+skipped3
            print('total_scheduled', total_scheduled)
            print('skipped1', skipped1)
            print('skipped2', skipped2)
            print('skipped3', skipped3)
            print('total_skipped', total_skipped)
            print('actually execed', total_scheduled-total_skipped)
        else:
            for gidx, cregs in enumerate(simulator_group_checks):
                tasks = []
                for nid, conds in simulators.items():
                    remaining = conds.difference(cregs)
                    
                    # skip check if a previous simgroup already removed all registers from simulators
                    if remaining == conds:
                        continue
                    
                    # skip check if a previous failed simgroup would have removed a subset of the current group
                    for prev_group in simulator_group_checks[:gidx]:
                        remaining_in_group = conds.union(prev_group)
                        if len(remaining_in_group) > 0 and remaining_in_group.issubset(cregs):
                            continue
                    
                    tasks.append((str(nid),
                                  cycles,
                                  " ".join(remaining),
                                  False,
                                  self.dryrun_prop_find_simulators,
                                  "",
                                  [],
                                  False)) # no glitches

                logging.info(f"checking whether {cregs} is required for simulation ({gidx} out of {len(simulator_group_checks)})")
                results = self.__run_multi(self.__check_diff_nids_against_cregs, tasks)
                for i, r in results.items():
                    nid = int(tasks[i][0])
                    # if still unsat, then its fine to remove it for real
                    if not results[i]:
                        for creg in cregs:
                            simulators[nid].remove(creg)

        bad = set()
        # sanity checks for simulatability
        for nid, conds in simulators.items():
            if len(conds) == 0:
                logging.warning(f"We apparently found a public value {self.circuit.cells[nid]}")
                bad.add(nid)
            if len(conds) == len(all_ctrct_cregs):
                logging.warning(f"Seems like we found a unsimulatable value {self.circuit.cells[nid]}")
                bad.add(nid)

        cfg_path = self.output + f"-simulators-{len(self.node_pvars)}-cycles.cfg"
        with open(cfg_path, "w+") as f:
            f.write(f"sanity hash {self.cfg.circ_hash}\n")
            for nid, sims in simulators.items():
                self.__update_simulators(nid, sims)
                f.write(f"mapping cell {nid} {' '.join(sorted(sims))}\n")

        return len(bad) == 0, ignored.union(simulators.keys())

    def __update_simulators(self, nid, cregs):
        if type(nid) == str: nid = int(nid)
        if nid not in self.node_simulators:
            self.node_simulators[nid] = set()
        self.node_simulators[nid].update(cregs)
        
    def __get_predecessor_cone(self, nid):
        yield nid
        if self.circuit.cells[nid].type in REGISTER_TYPES:
            return
        for pred in self.circuit.all_predecessors(nid):
            yield from self.__get_predecessor_cone(pred)

    def check_prop_limited(self):
        success = self.__check_equiv_specified()
        if not success: logging.error("checking existing equality spec failed"); return False
        #return True # TODO REMOVE

        done = set()
        # Validate that everything from the configuration file is correct
        success, done = self.__check_prop_specified(done)
        if not success: logging.error("checking existing spec failed")

        # For now, find anything that is absolutely not simulatable so we can fix it
        success, done = self.__check_prop_simulatable(done)
        if not success: logging.error("checking simulatable values failed")

        for nid in self.circuit.nodes:
            if self.circuit.cells[nid].type not in REGPORT_TYPES:
                done.add(nid)

        # Out of the simulatable stuff, find everything that is public
        success, done = self.__find_prop_public(done)
        if not success: logging.error("checking public values failed")

        # Find the simulating variables for all remaining nodes
        success, done = self.__find_prop_simulators(done)
        if not success: logging.error(f"inference of simulators incomplete")

        return success

    def __prepare_propagation(self, cycle):
        self.__find_glitch_registers(cycle)
        self.build_hwmodel_cycle(cycle, 0)
        assert(self.solver.num_scopes() == 0)
        self.solver.push()
        self.align_contract()
        self.__assert_constraints()

    def check_propagation(self):
        # load the contract smt
        self.load_contract_files("_contract_model.smt2")
        # generate hw model without a pre-cycle for transition leakage
        for cycle in range(0, self.ncycles):
            self.__prepare_propagation(cycle)
            logging.info(f"checking propagation for {cycle + 1}-cycle instructions")

            valid = self.check_prop_limited()
            self.solver.pop()
            if not valid: return valid

        logging.info(f"done with propagation checking, writing back results")
        with open(self.output + "-finding-propagation.cfg", "w+") as f:
            f.write(f"sanity hash {self.cfg.circ_hash}\n")
            publics = set()
            simulatables = []
            for nid, sims in self.node_simulators.items():
                if len(sims) == 0: publics.add(nid)
                else: simulatables.append(nid)

            covered = set()
            key_fun = (lambda x: (x[0].startswith("_") & 1) * 100000 - len(x[1]))
            wire_order = sorted(self.cfg.hw_name_ids.items(), key=key_fun)
            for hw_name, bits in wire_order:
                if covered.issuperset(bits):
                    continue
                if all(map(lambda x: x in publics, bits)):
                    f.write(f"mapping wire {hw_name} public\n")
                    covered.update(bits)
                    continue
                if len(bits) <= 0 or bits[0] not in self.node_simulators:
                    continue
                ref_simulators = self.node_simulators[bits[0]]
                if all(map(lambda x: x in self.node_simulators and self.node_simulators[x] == ref_simulators, bits)):
                    f.write(f"mapping wire {hw_name} {' '.join(ref_simulators)}\n")
                    covered.update(bits)
            other_pub = publics.difference(covered)
            for nid in sorted(other_pub):
                f.write(f"mapping cell {nid} public\n")
            for nid in sorted(set(simulatables).difference(covered)):
                f.write(f"mapping cell {nid} {' '.join(self.node_simulators[nid])}\n")
        return True

    def check_regs_limited(self):
        pairs = []
        pairs += [(hwreg, creg) for (hwreg, creg) in self.cfg.state_imp_hwsw.items()]
        pairs += [(hwreg, creg) for (creg, hwreg) in self.cfg.state_map_swhw.items() if "PC" not in creg]
        last_cycle = max(self.node_pvars.keys())
        cycle_pvars = self.node_pvars[last_cycle]

        for hwreg, creg in pairs:
            # get the correct ids, that is, those of predecessors
            hwreg_bits = self.cfg.hw_name_ids[hwreg]
            nids_ = [tuple(self.circuit.predecessors(reg_bit_id)) for
                     reg_bit_id in hwreg_bits]
            assert (all(map(lambda x: len(x) == 1, nids_)))
            nids = [nid_[0] for nid_ in nids_]

            hwreg_pvars = [cycle_pvars[nid][0] for nid in nids]
            k_ty = self.cfg.ctrct_state_var[creg]
            assert(len(hwreg_pvars) <= k_ty.length)
            creg_bvvar = self.pvars_contract(f"{creg}_final", k_ty)[0]
            creg_pvars = [smt_extract_bool(creg_bvvar, i) for i in range(k_ty.length)]
            print(f"Checking {creg} != {hwreg}")

            cond = smt.Or(*[smt.Xor(a, b) for a, b in zip(hwreg_pvars, creg_pvars)])
            res = self.solver.check(cond)
            if res == smt.sat:
                self.print_finding("Found counterexample", "wtf-error")
                return False
            else:
                print(f" Proven that {creg} == {hwreg}")
        return True

    def check_reg_mappings(self):
        # generate hw model without a pre-cycle for transition leakage
        for cycle in range(0, self.ncycles):
            self.__prepare_propagation(cycle)
            logging.info(f"checking equivalence for {cycle + 1}-cycle instructions")
            valid = self.check_regs_limited()
            self.solver.pop()
            if not valid: return valid
        return True

    def check_invar_limited(self):
        assert(self.mode == STABLE)

        # all cells which are labeled as secret dependent don't need to be checked
        assumed = set()
        for name in self.cfg.hw_state_var:
            assumed.update(self.cfg.hw_name_ids[name])

        # do we check all cycles or only the last?
        start_cycle = 0 # = len(self.node_pvars) - 1

        for cycle in range(start_cycle, max(self.node_pvars.keys())):
            # collects cells which are not marked as secret dependent (also those without labels)
            remaining = [(str(nid), str(cycle), "", True) for nid in self.circuit.nodes if
                         (nid not in assumed and (self.circuit.cells[nid].type in REGISTER_TYPES))]
            wires_indep_a = self.__run_multi(self.__check_diff_nids_against_cregs, remaining, do_all=True)
            assert (len(remaining) == len(wires_indep_a))
            bad = any(map(lambda i: wires_indep_a[i], range(len(remaining))))
            if bad: return False
        return True

    def __check_executable(self):
        forbidden = []
        while True:
            self.solver.push()
            for f in forbidden: self.solver.add(f)

            res = self.solver.check()
            assert(res != smt.unknown)
            if res == smt.unsat:
                self.solver.pop()
                return

            model = self.solver.model()
            curr_pvars = self.node_pvars[0]
            opcode_ids = self.cfg.hw_name_ids[self.cfg.hw_opcode_name]
            opcode = self.get_register_diff_str(curr_pvars, opcode_ids, model)

            disass_text = ""
            if self.disassembler is not None:
                disass_text = self.disassembler.disassemble(int(opcode, 2))
            logging.info(f"opcode = 0b{opcode} {disass_text}")

            revop = opcode[::-1]

            # slightly optimized check so that operations hopefully do not repeat
            important = list(range(0, 7))
            if not any(map(lambda x: x in disass_text, ("auipc", "lui", "jal", "j", "jalr"))):
                important += list(range(12, 15))
                if (revop.startswith("1100110") or # R-Type
                    any(map(lambda x: x in disass_text, ("slli", "srli", "srai")))):
                    important += list(range(25, 32))

            # important = [0, 1, 2, 3, 4, 5, 6, 12, 13, 14, 30]
            conds = {i:curr_pvars[opcode_ids[i]][0] for i in important}
            conds = [c if revop[i] != "1" else smt.Not(c) for i,c in conds.items()]
            cond = functools.reduce(smt.Or, conds[1:], conds[0])
            forbidden.append(cond)
            self.solver.pop()
        pass

    def check_invariant(self):
        for cycle in range(0, self.ncycles):
            self.__prepare_propagation(cycle)
            logging.info(f"checking invariants for {cycle+1} cycles")
            valid = self.check_invar_limited()
            self.solver.pop()
            if not valid: return valid
        return True
    
    def __check_possible(self, vars: str):
        vars_list = [smt.Bool(x, exists=True) for x in vars.split(' ')]
        
        #start = time.time()
        r = self.solver.check(vars_list)
        #end = time.time()
        #print('total time', end-self.debug_time)
        #self.debug_time = end
        #print('solver time', end-start)
        
        ret = 10 if (r == smt.sat) else 20
        return ret
        
    def list_executable_instrunstions(self):
        for cycle in range(0, self.ncycles):
            self.__prepare_propagation(cycle)
            # check which instructions can get executed in this limit
            logging.debug(f"listing valid {cycle+1}-cycle instructions")
            self.__check_executable()
            self.solver.pop()
            
    def __check_leak_implication(self, leak_a, leak_b, debug=False):
        assumptions = [
            smt.Bool(self.ctrct_substitutes[f"leak{leak_a}_is_active_final{SUFFIXES[0]}"]),
            smt.Not(smt.Bool(self.ctrct_substitutes[f"leak{leak_b}_is_active_final{SUFFIXES[0]}"])),
        ]
        r = self.solver.check(assumptions)
        return 10 if (r == smt.sat) else 20
            
    def construct_leak_implications(self):
        self.solver.push()
        leak_ids = list(self.leak_statements.keys())
        checks = [(la, lb, False) for la in leak_ids for lb in leak_ids][:]
        results = self.__run_multi(self.__check_leak_implication, checks)
        #results = {} # TODO reenable
        self.solver.pop()
        
        implications = {la: set() for la in leak_ids}
        tree = helpers.ImpTree()
        G = nx.Graph()
        G.add_nodes_from(leak_ids)
        for r_id, r_val in results.items():
            if not r_val:
                la, lb, _ = checks[r_id]
                implications[la].add(lb)
        for la, lbs in implications.items():
            print(la, sorted(lbs))
        print(implications)
        for la, lbs in implications.items():
            for lb in lbs:
                if la in implications[lb]:
                    G.add_edge(la, lb)
        
        cliques = list(nx.clique.find_cliques(G))
        for clq in cliques:
            print(clq, implications[clq[0]])
            
        assert(sum(len(x) for x in cliques) == len(leak_ids)), 'nodes in multiple cliques'
        
        cliques_by_id = {c[0]: c for c in cliques}
        clique_ids = cliques_by_id.keys()
        
        implication_graph = nx.DiGraph()
        implication_graph.add_nodes_from(clique_ids)
        for cid in clique_ids:
            for impl in implications[cid]:
                if impl in clique_ids and cid != impl:
                    implication_graph.add_edge(impl, cid)
                       
        print('---')
        for node in implication_graph.nodes:
            out = f'{node}:'
            for st, end in implication_graph.edges(node):
                if st != node: continue
                out += f' {end}'
            print(out)
        print('---')
        
        implication_graph = nx.transitive_reduction(implication_graph)
            
        nodes = {c: helpers.ImpTree(cliques_by_id[c]) for c in clique_ids}
        for c1, c2 in implication_graph.edges:
            nodes[c1].add_child(nodes[c2])
            
        roots = [n for n in nodes.values() if n.parent == None]
        
        for root in roots:
            print(root)
        
        self.leak_implication_tree = helpers.ImpTree()
        for root in roots:
            self.leak_implication_tree.add_child(root)
        self.leak_implications = implications

    def __check_leak_group_intersection_deprecated(self, nids: list[int], cycles: str, leaks: list[int]):
        logging.debug(f'__check_leak_group_intersection nids={nids} cycles={cycles} leaks={leaks}')
        #self.solver.solver.set(unsat_core=True)
        assert len(nids) == 1
        nid = nids[0]
        assert len(leaks) >= 2
        leak_exprs = [self.leak_statements[l] for l in leaks]
        alt_leaks = leaks[1:]
        # check if they cover their unique regions

        leaks_triggered = ' '.join(f"leak{l}" for l in leaks)
        
        #state_vars = self.cfg.hw_state_var | {self.cfg.hw_opcode_name}
        ctstate_pvars = []
        for ct_var_name in self.cfg.ctrct_state_var.keys(): # TODO sanity check if this list is complete?
            k_ty = self.cfg.ctrct_state_var[ct_var_name]
            sw_smt_vars = self.pvars_contract(ct_var_name, k_ty)
            for bit_pos in range(0, k_ty.length):
                for dom_idx in range(NUM_CONTEXTS):
                    ctstate_pvars.append(smt_extract_bool(sw_smt_vars[dom_idx], bit_pos))
        
        cycle = 0 # TODO
        assert cycles == '-1 0'
        curr_pvars = self.node_pvars[cycle]
        opcode_ids = self.cfg.hw_name_ids[self.cfg.hw_opcode_name]
        hw_regs = [nid for nid in self.circuit.nodes if self.circuit.cells[nid].type in REGISTER_TYPES]
        hwstate_pvars = [y for x in hw_regs for y in curr_pvars[x]]
        state_pvars = hwstate_pvars + ctstate_pvars
        
        
        last_clause = None
        iteration = 1
        x = 0
        while True:
            logging.debug('----------- round', iteration)
            x+=1
            res = self.__check_diff_nids_against_cregs(str(nid), cycles, " ".join(leak_exprs[0]),
                False, self.dryrun_prop_simulatable, leaks_triggered)
            if res == 20:
                print(f'leak {leaks[0]} covers the remaining region')
                return True
            
            with open(f'dbg_{x}.smt', 'w') as f:
                f.write(self.solver.solver.to_smt2())

            model = self.solver.model()
            
            if last_clause is not None:
                assert model.model.eval(last_clause).sexpr() == 'true'
            
            opcode = self.get_register_diff_str(curr_pvars, opcode_ids, model)
            opvars = [curr_pvars[i][0] for i in opcode_ids]
            disass_text = ""
            if self.disassembler is not None:
                disass_text = self.disassembler.disassemble(int(opcode, 2))
            print(f'counterexample for leak {leaks[0]} found (opcode=0b{opcode} {disass_text})')
            
            state_ass = {x: model.eval(x, model_completion=True).sexpr() == 'true' for x in state_pvars}
            
            assignment_map = {}
            assignments = []
            for v, a in state_ass.items():
                ass = smt.Equals(v, smt.BoolVal(a))
                assignments.append(ass)
                assignment_map[v] = ass
                
            covering_clauses = []
            for alt_idx in range(1, len(leaks)):
                res = self.__check_diff_nids_against_cregs(str(nid), cycles, " ".join(leak_exprs[alt_idx]),
                    False, self.dryrun_prop_simulatable, leaks_triggered, assignments)
                if res == 20:
                    print(f'leak {leaks[alt_idx]} covers counterexample under condition:')
                    c = self.solver.solver.unsat_core()
                    print('core=', c)
                    new_clause = []
                    
                    for ass in c:
                        
                        new_clause.append(smt.smt.Not(ass))
                        print('adding new_clause entry', ass)
                            
                    covering_clause = smt.smt.Or(*new_clause)
                    print('adding clause', covering_clause)
                    covering_clauses.append(covering_clause)
                    last_clause = covering_clause
                    #break # restart search
            if len(covering_clauses) == 0:
                print(f'no other leak covers counterexample. intersection coverage check failed')
                return False
            
            for covering_clause in covering_clauses:
                self.solver.solver.add(covering_clause)
        print(opcode_ids)
        
    def leak_covers_own_region(self, nid: int, cycles: str, leak: int, covered_leak_regions: list[int]):
        leak_exprs = self.leak_statements[leak]
        res = self.__check_diff_nids_against_cregs(str(nid), cycles, " ".join(leak_exprs),
            False, self.dryrun_prop_simulatable, f"leak{leak}")
        return res == 20 # unsat -> covering
        
    def leak_region_covered_deprecated(self, nid: int, cycles: str, leak: int, covered_leak_regions: list[int]):
        if self.leak_covers_own_region(nid, cycles, leak, covered_leak_regions):
            print(f'leak {leak} covers own emission region')
            return True # self covering
        leak_group = list(self.leak_implications[leak]) + [leak] # TODO optimize order
        self.solver.push()
        res = self.__check_leak_group_intersection_deprecated([nid], cycles, leak_group)
        self.solver.pop()
        if res:
            print(f'leak {leak} emission region is sharedly covered')
            return True # covering together with implicated
        return False
        
    def leak_regions_complete(self, nid: int, cycles: str, covered_leak_regions: list[int]):
        print('checking if leak emission region is complete: ', covered_leak_regions)
        checks = [(str(nid), cycles, "", False, self.dryrun_prop_simulatable, " ".join(map(lambda x: f"-leak{x}", covered_leak_regions)))]
        results = self.__run_multi(self.__check_diff_nids_against_cregs, checks)
        return results[0]
        
    def check_leak_group(self, nids: list[int], cycles: list[int], leaks: list[int], debug=False):
        # setup
        opcode_ids = self.cfg.hw_name_ids[self.cfg.hw_opcode_name]
        
        #ctstate_diff_pvars = []
        ctstate_pvars = []
        for ct_var_name in self.cfg.ctrct_state_var.keys():
            k_ty = self.cfg.ctrct_state_var[ct_var_name]
            sw_smt_vars = self.pvars_contract(ct_var_name, k_ty)
            for bit_pos in range(0, k_ty.length):
                ctstate_pvars.append(tuple(smt_extract_bool(sw_smt_vars[dom_idx], bit_pos) for dom_idx in range(NUM_CONTEXTS)))

        assert 0 in cycles
        hwstate_pvars = []
        hwgates_pvars = []
        for cycle in sorted(self.node_pvars.keys()):
            curr_pvars = self.node_pvars[cycle]
            hw_regs = [nid for nid in self.circuit.nodes if self.circuit.cells[nid].type in REGPORT_TYPES] # TODO also ports + constants?
            hw_gates = [nid for nid in self.circuit.nodes]
            hwgates_pvars += [curr_pvars[x] for x in hw_gates]
            hwstate_pvars += [curr_pvars[x] for x in hw_regs]
            
        print(len(hwstate_pvars), len(hwgates_pvars))

        state_pvars = ctstate_pvars + hwstate_pvars
        hwstate_diff_pvars = [smt.Xor(*x) for x in hwstate_pvars]
        diff_pvars = hwstate_diff_pvars
        
        state_pvars = [x[:1] for x in state_pvars]
    
        # assert hardware difference: hw_A != hw_B
        node_diff_conds = []
        for cycle in cycles:
            cycle_pvars = self.node_pvars[cycle]
            pvars_list = [cycle_pvars[nid] for nid in nids]
            for pvars in pvars_list: node_diff_conds.append(smt.Xor(pvars[0], pvars[1]))
        self.solver.add(smt.Or(*node_diff_conds))
        
        leak_emitted = {leak: [smt.Bool(self.ctrct_substitutes[f"leak{leak}_is_active_final{suff}"], exists=True)
                               for suff in SUFFIXES]
                        for leak in leaks}
                
        final_clauses = []
                
        for iteration in itertools.count():
            logging.debug(f'----------- round {iteration}')
            sw_opc_len = self.cfg.ctrct_opcode_ty.length
            sw_opc_name = self.ctrct_substitutes[self.cfg.ctrct_opcode + "1"]
            sw_opc = smt.BitVec(sw_opc_name, sw_opc_len, exists=True)
            assert self.solver.num_scopes() == 2
            #res = self.solver.check() if (not self.dryrun) else smt.unsat
            # TODO implement leak implication order properly
            res = self.solver.check(smt.Or(*[smt.And(*leak_emitted[x]) for x in leaks if x != 1])) if (not self.dryrun) else smt.unsat
            if res == smt.unsat:
                print('leaf leaks are done')
                res = self.solver.check() if (not self.dryrun) else smt.unsat
            
            if res == smt.unsat:
                print(f'all regions covered')
                print('final proof:')
                for leak, cl in final_clauses:
                    print(leak)
                    for elem in cl.children:
                        print('  ', elem)
                print('/final proof')
                return True
            
            
            # minimize number of diffs
            for i in range(1, len(diff_pvars)+1):
                print(f'checking if {i} diffs are sufficient')
                restriction = smt.AtMost(*diff_pvars, i)
                res = self.solver.check(restriction)
                if res == smt.sat:
                    break


            # minimize number of diffs
            for i in range(1, len(diff_pvars)+1):
                print(f'checking if {i} diffs are sufficient')
                restriction = smt.AtMost(*diff_pvars, i)
                res = self.solver.check(restriction)
                if res == smt.sat:
                    break

            model = self.solver.model()
            # print opcode
            opcode = self.get_register_diff_str(curr_pvars, opcode_ids, model)
            opvars = [curr_pvars[i][0] for i in opcode_ids]
            disass_text = ""
            if self.disassembler is not None:
                disass_text = self.disassembler.disassemble(int(opcode, 2))
            print(f'uncovered counterexample found (opcode=0b{opcode} {disass_text})')
            
            # extract model assignments
            assignments = []
            assignments_inverted = []

            for (world_a,) in state_pvars:
                world_a_val = smt.BoolVal(model.evalBool(world_a))
                assignments.append(smt.Equals(world_a, world_a_val))
                assignments_inverted.append(smt.Equals(world_a, world_a_val)) # TODO remove
            self.solver.push()
            
            # check which leaks are active
            active_leaks = []
            for leak in leaks:
                if all(model.evalBool(b) for b in leak_emitted[leak]):
                    active_leaks.append(leak)
            print('active leaks:', active_leaks)

            covering_conditions = {}
            for leak in active_leaks[::-1]:
                print('checking leak', leak, flush=True)
                
                self.solver.push()
                assert self.solver.num_scopes() == 4
                
                # assert leak params are equal in both worlds
                cregs = self.leak_statements[leak]
                indep_names = [self.ctrct_substitutes[f"assert_indep_{creg}"] for creg in cregs]
                asserts = [(smt.Bool(cond, exists=True)) for cond in indep_names if cond != ""]
                
                # TODO check asserts in model, if sat, skip solver call
                
                for a in asserts:
                    self.solver.add(a)
                    
                for a in leak_emitted[leak]:
                    self.solver.add(a)
                
                res = self.solver.check(assignments) if (not self.dryrun) else smt.unsat
                
                if res == smt.unsat:
                    print(f'leak {leak} covers counterexample under condition:')
                    c = self.solver.unsat_core()
                    #print('core=', c.core)
                    
                    covering_conditions[leak] = []
                    for i, ass in enumerate(assignments):
                        if ass in c:
                            covering_conditions[leak].append(ass)
                    for clause in covering_conditions[leak]:
                        print('  ', clause)
                self.solver.pop()
            self.solver.pop()
            
            if len(covering_conditions) == 0:
                print(f'no other leak covers counterexample. coverage check failed')
                return False
            
            for condition_list in [covering_conditions]:
                for leak, conditions in condition_list.items():
                    new_clause = [smt.Not(x) for x in leak_emitted[leak]]
                    for cond in conditions:
                        new_clause.append(smt.Not(cond))
                    cl = smt.Or(*new_clause)
                    self.solver.add(cl)
                    final_clauses.append((leak, cl))
        
    def check_comb_limited_multileak(self, nids: list[int], cycles: list[int], leaks: list[int], debug=False):
        self.solver.push()
        res = self.check_leak_group(nids, cycles, leaks, debug)
        self.solver.pop()
        return 10 if res else 20
    
    def check_leak_covered_simple(self, nids, cycles: list[int], cregs="", debug=False, dryrun=False, actives="", assumptions=[]):
        args = (nids, ' '.join(str(x) for x in cycles), cregs, debug, dryrun, actives, assumptions)
        
        for nid, leak_id, condition in parallel.new_messages():
            
            actives = f'-leak{leak_id}'
            leak_exprs = self.leak_statements[leak_id]
            
            indep_names = [self.ctrct_substitutes[f"assert_indep_{creg}"] for creg in leak_exprs]
            cond_bools = [(smt.Bool(cond, exists=True)) for cond in indep_names if cond != ""]
            leak_violated_bools = [smt.Not(x) for x in cond_bools]
            
            # These are additional leak activity constraints
            active_bools = []
            for act in actives.split():
                neg = False
                if act.startswith("-"):
                    act = act[1:]
                    neg = True
                act_bools = [smt.Bool(self.ctrct_substitutes[f"{act}_is_active_final{suff}"], exists=True) for suff in SUFFIXES]
                if neg: act_bools = [smt.Not(b) for b in act_bools]
                active_bools += act_bools
            
            node_eq_conds = []
            for cycle in cycles:
                cycle_pvars = self.node_pvars[cycle]
                pvars_list = [cycle_pvars[nid]]
                for pvars in pvars_list: node_eq_conds.append(smt.Equals(pvars[0], pvars[1]))
            self.solver.add(smt.Or(*node_eq_conds, *active_bools, *leak_violated_bools))
            self.solver.dbg_learn(smt.Or(*node_eq_conds, *active_bools, *leak_violated_bools))
            
        return self.__check_diff_nids_against_cregs(*args)

    def check_comb_limited(self):
        
        def to_leak_list(li: list[int]) -> list[str]:
            return [f"leak{leak_id}" for leak_id in li]
        
        first_cycle = 0
        last_cycle = max(self.node_pvars.keys())
        duration = last_cycle - first_cycle + 1
        # at this point the contract validity is always asserted
        leak_simulators = {}
        ret = True
        broken = set()
        nodes = [nid for nid in self.circuit.nodes if self.circuit.cells[nid].type not in REGISTER_TYPES]
        leak_ids = list(self.leak_statements.keys())
        reachable_leaks = leak_ids

        strategy = 'flat'
        debug = False
        with_glitches = True # TODO calculate properly
            
        # check which leaks are reachable in the current cycle
        checks = [(' '.join(self.ctrct_substitutes[f"leak{leak_id}_is_active_final{suff}"] for suff in SUFFIXES),)
                   for leak_id in leak_ids]
        results = self.__run_multi(self.__check_possible, checks)
        reachable_leaks = sorted([leak_ids[i] for i, res in results.items() if res])
        
        logging.info(f'Reachable leaks: {" ".join(str(x) for x in reachable_leaks)}')
        logging.info(f'Unreachable leaks: {" ".join(str(x) for x in (self.leak_statements.keys() - reachable_leaks))}')
        
        def get_leak_simulators(cycle, nid):
            res = leak_simulators.get((cycle, nid))
            if not res:
                return []
            return res

        for cycle in range(first_cycle, last_cycle + 1):
            logging.info(f"Running checks for cycle {cycle}")
            cycles_list = [cycle - 1, cycle] if self.mode == TRANSIENT else [cycle]
            cycles = " ".join(map(str, cycles_list))
            
            # ignore nids defined as public in config
            cfg_leak_simulators = self.cfg.get_leak_simulators(duration, cycle)
            processed = [nid for nid in nodes if nid in cfg_leak_simulators and cfg_leak_simulators[nid] == []]
            remaining = [nid for nid in nodes if nid not in processed]

            logging.info(f"Filtering public nodes for cycle {cycle}")
            # first check if stuff is simulatable without leak statements i.e. public
            checks = [(str(nid), cycles, "", False, self.dryrun_prop_simulatable)
                      for nid in remaining]
            results = self.__run_multi(self.__check_diff_nids_against_cregs, checks)
            remaining = [int(checks[r_id][0]) for r_id, r_val in results.items() if r_val]
            processed += [int(checks[r_id][0]) for r_id, r_val in results.items() if not r_val]
            
            for nid in processed:
                leak_simulators[(cycle, nid)] = None
            for nid in processed:
                leak_simulators[(cycle, nid)] = []

            if strategy == 'flat':
                priority_queue = sorted(self.leak_statements.keys())[::-1]

                for leak_id in priority_queue:
                    if leak_id not in reachable_leaks: continue
                    if len(remaining) == 0: break
                    logging.info(f"Checking {len(remaining)} nodes against simulator leak{leak_id}")
                    leak_exprs = self.leak_statements[leak_id]
                    checks = [(str(nid),
                               cycles,
                               " ".join(leak_exprs),
                               debug,
                               self.dryrun_prop_simulatable,
                               f"leak{leak_id}",
                               [],
                               with_glitches)
                            for nid in remaining]
                    results = self.__run_multi(self.__check_diff_nids_against_cregs, checks)
                    simulated = [int(checks[r_id][0]) for r_id, r_val in results.items() if not r_val]
                    remaining = [int(checks[r_id][0]) for r_id, r_val in results.items() if r_val]
                    for nid in simulated:
                        if (cycle, nid) not in leak_simulators:
                            leak_simulators[(cycle, nid)] = []
                        leak_simulators[(cycle, nid)].append(leak_id)
                    if len(simulated):
                        logging.info(f"Checking {len(simulated)} nodes for completeness")
                        checks = [(str(nid),
                                   cycles,
                                   "",
                                   debug,
                                   self.dryrun_prop_simulatable,
                                   " ".join(map(lambda x: f"-{x}", to_leak_list(leak_simulators[(cycle, nid)]))),
                                   [],
                                   with_glitches)
                                for nid in simulated]
                        results = self.__run_multi(self.__check_diff_nids_against_cregs, checks)
                        remaining += [int(checks[r_id][0]) for r_id, r_val in results.items() if r_val]
                   
            if strategy == 'deductive':
                priority_queue = sorted(self.leak_statements.keys())[::-1]
                
                for leak_id in priority_queue:
                    if leak_id not in reachable_leaks: continue
                    if len(remaining) == 0: break
                    logging.info(f"Checking {len(remaining)} nodes against simulator leak{leak_id}")
                    leak_exprs = self.leak_statements[leak_id]
                    
                    scheduled = set()
                    done = set()
                    simulated = set()
                    
                    mp = parallel.Multipipe()
                    
                    skipped = 0
                
                    def schedule(nid, add_to_queue, add_result=None):
                        nonlocal skipped
                        if nid in scheduled or nid not in remaining:
                            return
                        # delay schedule until all predecessors are done:
                        for pred in self.circuit.all_predecessors(nid):
                            if pred in remaining and pred not in done:
                                return
                            
                        deduced = False
                        if self.circuit.cells[nid].type == PORT_TYPE:
                            pass # ports can never be deduced
                        elif cycles == '0' and self.circuit.cells[nid].type in REGPORT_TYPES: # TODO check if this is correct
                            pass # regports cannot be decuced in first cycle
                        else:
                            # skipt check if all predecessors can be simulated with a subset of the remaining simulators
                            for pred in self.circuit.all_predecessors(nid):
                                #if leak_id not in get_leak_simulators(cycle, pred):
                                if pred not in simulated:
                                    break
                            else:
                                skipped += 1
                                deduced = True
                                
                        args = (str(nid), cycles_list, " ".join(leak_exprs),
                                False, False, f"leak{leak_id}")
                        if deduced:
                            add_result(args, smt.unsat)
                        else:
                            add_to_queue(args)
                        assert nid not in scheduled
                        scheduled.add(nid)
                    
                    def on_result(args, res, add_to_queue, add_result):
                        nid = int(args[0])
                        assert res in [smt.sat, smt.unsat]
                        if res == smt.unsat:
                            simulated.add(nid)
                        assert nid not in done, f'{nid} added twice'
                        done.add(nid)
                        for succ in self.circuit.all_successors(nid):
                            schedule(succ, add_to_queue, add_result)
                        
                    initial_nodes = self.__find_initial_nodes([x for x in self.circuit.nodes if x not in remaining])
                    initial_queue = []
                    for nid in initial_nodes:
                        schedule(nid, initial_queue.append)
                    
                    results = parallel.run_multi_interactive(self.check_leak_covered_simple, initial_queue, on_result, )#multipipe=mp)
                    assert done == scheduled == set(remaining), f'mismatch in tasks: {scheduled.difference(done)}'
                    
                    for nid in simulated:
                        if (cycle, nid) not in leak_simulators:
                            leak_simulators[(cycle, nid)] = []
                        leak_simulators[(cycle, nid)].append(leak_id)
                    
                    print('++++++++++++++++++++++++++++++++++++++++++++ skipped:', skipped)

                    if len(simulated):
                        logging.info(f"Checking {len(simulated)} nodes for completeness")
                        checks = [(str(nid), cycles, "", False, self.dryrun_prop_simulatable, " ".join(map(lambda x: f"-{x}", to_leak_list(leak_simulators[(cycle, nid)]))), (), with_glitches)
                                for nid in simulated]
                        results = self.__run_multi(self.__check_diff_nids_against_cregs, checks)
                        for r_id, r_val in results.items():
                            if not r_val:
                                remaining.remove(int(checks[r_id][0]))
                        
                    mp.close()
                    
            with open(TMP_DIR + f"/leaksim-{duration}-covcheck1.cfg", "w+") as f:
                for kid, leaks in leak_simulators.items():
                    directive = "leaksim" if kid[1] not in remaining else "// broken"
                    leaks_str = "public" if leaks is None else " ".join(str(x) for x in leaks)
                    f.write(f"{directive} {duration} {kid[0]} {kid[1]} {leaks_str}\n")
                   
            if strategy == 'multileak':
                checks = []
                for nid in remaining:
                    unfinished_leaks = reachable_leaks # default to all
                    if (cycle, nid) in leak_simulators:
                        finished_leaks = [x for x in reachable_leaks if x in leak_simulators[(cycle, nid)]]
                        unfinished_leaks = [x for x in reachable_leaks if x not in leak_simulators[(cycle, nid)]]
                    checks.append(([nid], cycles_list, unfinished_leaks, self.debug))
                results = self.__run_multi(self.check_comb_limited_multileak, checks)
                remaining = [int(l) for r_id, r_val in results.items() if not r_val for l in checks[r_id][0]]
            
            if len(remaining):
                ret = False
                for nid in remaining:
                    if (cycle, nid) not in leak_simulators:
                        leak_simulators[(cycle, nid)] = []
                broken.update({(cycle, nid) for nid in remaining})
                break

        with open(TMP_DIR + f"/leaksim-{duration}.cfg", "w+") as f:
            for kid, leaks in leak_simulators.items():
                directive = "leaksim" if kid not in broken else "// broken"
                leaks_str = "public" if leaks is None else " ".join(str(x) for x in leaks)
                f.write(f"{directive} {duration} {kid[0]} {kid[1]} {leaks_str}\n")
        return ret


    def check_combination(self):
        self.load_contract_files("_contract_model.smt2")

        # build the transient cycle
        first_cycle = -1 if self.mode in (TRANSIENT, GLITCH) else 0
        if self.mode in (TRANSIENT, GLITCH):
            self.__find_glitch_registers(-1)
            self.build_hwmodel_cycle(-1, first_cycle)
            self.make_precycle_implications()
            self.make_precycle_equalities()

        for cycle in range(0, self.ncycles):
            logging.info(f"checking leakage of {cycle+1}-cycle instructions")
            self.build_hwmodel_cycle(cycle, first_cycle)
            self.solver.push()
            self.align_contract()
            self.__assert_constraints()
            self.solver.simplify()
            valid = self.check_comb_limited()
            assert self.solver.num_scopes() == 1
            self.solver.pop()
            if not valid: return valid
        return True
