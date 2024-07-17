import logging

import helpers
from classes import CfgZ3Typ, SignalConstraint
from defines import *
import sys, json
from CircuitGraph import net_names
from typing import Union
from collections import defaultdict

class ContractParseException(Exception):
    def __init__(self, message):
        super().__init__(message)

class ContractCfg:
    def __init__(self, config_paths, json_path, top_module, ncycles, debug_nodes=None):
        self.ctrct_state_var  = {} # variable/sensitive state (e.g. registers) of a contract
        self.hw_state_var     = set() # variable/sensitive state (e.g. registers) of hardware
        self.hw_state_pub     = set() # secret independent state (e.g. registers) of hardware
        self.hw_state_debug   = set() # signals that should be printed on a finding
        self.hw_one_hots      = set() # hw signals for which only a single wire/bit can be high/one
        self.hw_equivs        = set() # a set of tuples for hardware signals that must be equivalent
        self.hw_ignores       = set() # a set of signals that are ignored during combination verification
        self.state_map_memory = {} # a set of fairness contstraints
        self.state_map_cellsw = {} # map between cells and contract variables for transient case
        self.state_map_swhw   = {}
        self.state_imp_hwsw   = {}
        self.hw_consts        = {} # map of hw names to their constant value
        self.ctrct_opcode     = ""
        self.ctrct_opcode_ty  = ""
        self.hw_opcode_name   = ""
        self.circ_hash = helpers.file_hash(json_path)
        self.simulation_groups: list[tuple[str]] = [] # groups of contract varables
        self.leak_simulators: map[int, map[int, map[int, list[int]]]] = {} # duration -> cycle -> node_id -> list[leak_id]
        # map between contract registers/leakagestate and their equivalences in hardware
        self.hwct_equivs = defaultdict(lambda: defaultdict(lambda: set())) # ct_var -> cycle -> hw_var
        with open(json_path, "r") as f:
            circuit_json = json.load(f)
        self.hw_name_ids = net_names(circuit_json["modules"][top_module])
        self.ncycles = ncycles
        for cfg_path in config_paths:
            self.parse_config(cfg_path)
        self.debug_nodes = debug_nodes
            
    def get_leak_simulators(self, duration: int, cycle: int = None, node_id: int= None) -> Union[list[int], None]:
        dur_map = self.leak_simulators.get(duration)
        if dur_map is None or cycle is None: return dur_map if dur_map else {}
        cyc_map = dur_map.get(cycle)
        if cyc_map is None or node_id is None: return cyc_map if cyc_map else {}
        return cyc_map.get(node_id)

    def add_ctrct_var(self, ident, typ: CfgZ3Typ):
        if ident in self.ctrct_state_var:
            raise ContractParseException(f"duplicate addition of variable {ident} to contract state.")
        else:
            self.ctrct_state_var[ident] = typ

    def add_ctrct_opcode(self, ident, typ):
        if self.ctrct_opcode != "":
            raise ContractParseException(f"duplicate addition of contract opcode.")
        else:
            self.ctrct_opcode    = ident
            self.ctrct_opcode_ty = typ

    def add_ctrct_simgroup(self, ctrct_vars):
        ctrct_vars = tuple(sorted(ctrct_vars))
        if ctrct_vars in self.simulation_groups:
            raise ContractParseException(f"duplicate definition of simulation group {ctrct_vars}.")
        else:
            self.simulation_groups.append(ctrct_vars)

    def __intern_swhw_checks(self, isw, ihw):
        if isw not in self.ctrct_state_var:
            raise ContractParseException(f"Undefined software variable {isw}")

        if ihw not in self.hw_state_var and ihw not in self.hw_state_pub:
            raise ContractParseException(f"Undefined hardware variable/public {ihw}")

    def add_reg_mapping(self, isw, ihw):
        self.__intern_swhw_checks(isw, ihw)
        # check if ihw has already been mapped
        if (ihw in self.state_map_swhw.values() or
                ihw in self.state_imp_hwsw.keys() or
                isw in self.state_map_swhw.keys()):
            logging.error(f"mapping conflict.")
            sys.exit(EXIT_CODE_UNREACHABLE)
        self.state_map_swhw[isw] = ihw

    def add_ls_mapping(self, isw, ihw):
        self.__intern_swhw_checks(isw, ihw)
        # check if ihw has already been mapped
        if (ihw in self.state_map_swhw.values() or
            ihw in self.state_imp_hwsw.keys()):
            logging.error(f"mapping conflict.")
            sys.exit(EXIT_CODE_UNREACHABLE)
        self.state_imp_hwsw[ihw] = isw

    def add_hw_state_debug(self, name):
        if name not in self.hw_name_ids:
            raise ContractParseException(f"hardware signal {name} does not exist")
        self.hw_state_debug.add(name)

    def add_hw_state_pub(self, name):
        if name not in self.hw_name_ids:
            raise ContractParseException(f"hardware signal {name} does not exist")
        if name in self.hw_state_var:
            raise ContractParseException(f"hardware signal {name} already defined as variable")
        self.hw_state_pub.add(name)

    def add_hw_state_var(self, name):
        if name not in self.hw_name_ids:
            raise ContractParseException(f"hardware signal {name} does not exist")
        if name in self.hw_state_pub:
            raise ContractParseException(f"hardware signal {name} already defined as constant")
        self.hw_state_var.add(name)

    def add_hw_one_hot(self, ident, start, end):
        if ident in self.hw_state_var:
            raise ContractParseException(f"{ident} has been declared as variable")
        else:
            self.hw_one_hots.add(SignalConstraint(start, end, [ident]))

    def add_cellmapping(self, cell, maps):
        if type(cell) == str: cell = int(cell)
        if cell in self.state_map_cellsw:
            raise ContractParseException(f"duplicate cell mapping of {cell}.")
        else:
            self.state_map_cellsw[cell] = maps

    def add_cellmapping_name(self, name, maps):
        if name not in self.hw_name_ids:
            raise ContractParseException(f"hardware signal {name} does not exist")
        ids = self.hw_name_ids[name]
        for cell in ids:
            # allow duplicate mapping to public
            if cell in self.state_map_cellsw and self.state_map_cellsw[cell] != maps and maps != "public":
                raise ContractParseException(f"duplicate cell mapping of {cell} in {name} consisting of {self.hw_name_ids[name]}.")
            else:
                self.state_map_cellsw[cell] = maps

    def add_equivmapping_name(self, cycle_annotation, name: str, maps: str):
        #if name not in self.hw_name_ids:
        #    raise ContractParseException(f"hardware signal {name} does not exist")
        self.hwct_equivs[name][cycle_annotation].add(maps)

    def add_hw_opcode(self, name):
        if name not in self.hw_name_ids:
            raise ContractParseException(f"hardware signal {name} does not exist")
        if name in self.hw_state_var:
            raise ContractParseException(f"hardware signal {name} already defined as variable")
        self.hw_opcode_name = name

    def add_hw_const(self, hwnm, val, start, end):
        if hwnm in self.hw_state_var:
            logging.warn(f"{hwnm} has been declared as variable")
            # raise ContractParseException
        if (not val.startswith("0b") or
              len(val) < 3 or
              any(map(lambda x: x != "0" and x != "1", val[2:]))):
            raise ContractParseException("binary string is invalid")
        bits = [x == "1" for x in val[2:]]
        if hwnm not in self.hw_consts:
            self.hw_consts[hwnm] = set()
        self.hw_consts[hwnm].add(SignalConstraint(start, end, bits))

    def add_hw_equiv(self, name1, name2, start, end):
        for name in (name1, name2):
            if name not in self.hw_name_ids:
                raise ContractParseException(f"hardware signal {name} does not exist")
        if len(self.hw_name_ids[name1]) != len(self.hw_name_ids[name2]):
            logging.warning(f"hardware signals {name1} and {name2} have different lengths")
        self.hw_equivs.add(SignalConstraint(start, end, (name1, name2)))

    def add_hw_ignore(self, name):
        if name not in self.hw_name_ids:
            raise ContractParseException(f"hardware signal {name} does not exist")
        self.hw_ignores.add(name)

    def parse_contract_directive(self, ln, directive, d_args):
        functions = {"register": self.add_ctrct_var,
                     "leakagestate": self.add_ctrct_var,
                     "opcode": self.add_ctrct_opcode,
                     "simgroup": self.add_ctrct_simgroup}
        if directive == "simgroup":
            return functions[directive](d_args)
        if directive not in functions or len(d_args) != 3:
            logging.error(f"line {ln}: invalid contract directive in config file: {directive}")
            raise ContractParseException
        functions[directive](d_args[0], CfgZ3Typ(" ".join(d_args[1:])))

    def parse_mapping_directive(self, ln, directive, d_args):
        dir_anot = directive.split('@')
        directive, annotations = dir_anot[0], dir_anot[1:]
            
        if directive == 'equiv':
            if len(annotations) != 1:
                raise ContractParseException(f"line {ln}: must specify cycle (equiv@<cycle>): {directive}")
        else:
            if len(annotations) != 0:
                raise ContractParseException(f"line {ln}: no cycle annotation allowed (equiv@<cycle>): {directive}")
            
        # TODO: resolve this like the others
        valid_keywords = {"register": 2, "leakagestate": 2, "wire": 2, "cell": 2, "equiv": 2}
        if directive not in valid_keywords:
            raise ContractParseException(f"line {ln}: invalid mapping directive in config file: {directive}")

        match directive:
            case "register" | "leakagestate":
                if valid_keywords[directive] != len(d_args):
                    raise ContractParseException(f"line {ln}: invalid mapping directive in config file: {directive}")
                # register mapping
                ctrct, hw = d_args
                if directive == "register":
                    self.add_reg_mapping(ctrct, hw)
                else:
                    self.add_ls_mapping(ctrct, hw)
            case "cell":
                if valid_keywords[directive] > len(d_args):
                    raise ContractParseException(f"line {ln}: invalid mapping directive in config file: {directive}")
                # mapping cell [hwid] [public|ctrt_var+]
                self.add_cellmapping(d_args[0], d_args[1:])
            case "wire":
                if valid_keywords[directive] < len(d_args):
                    raise ContractParseException(f"line {ln}: invalid mapping directive in config file: {directive}")
                if d_args[0] not in self.hw_name_ids:
                    raise ContractParseException(f"hardware signal {d_args[0]} does not exist")
                # mapping wire [hwname] [public|ctrt_var+]
                self.add_cellmapping_name(d_args[0], d_args[1:])
            case "equiv":
                if valid_keywords[directive] != len(d_args):
                    raise ContractParseException(f"line {ln}: invalid mapping directive in config file: {directive}")
                    
                annot = self._parse_anot(annotations[0], ln, directive)
                    
                self.add_equivmapping_name(annot, d_args[0], d_args[1])

    def parse_leaksim_directive(self, ln, directive, d_args):
        
        if len(d_args) < 3:
            raise ContractParseException(f"line {ln}: invalid leaksim directive")
        
        duration = int(directive)
        cycle = int(d_args[0])
        node = int(d_args[1])
        if d_args[2] == 'public':
            leaks = []
        else:
            leaks = [int(x) for x in d_args[2:]]
        
        if duration not in self.leak_simulators:
            self.leak_simulators[duration] = {}
        if cycle not in self.leak_simulators[duration]:
            self.leak_simulators[duration][cycle] = {}
        if node not in self.leak_simulators[duration][cycle]:
            self.leak_simulators[duration][cycle][node] = {}
        self.leak_simulators[duration][cycle][node] = leaks
        
    def _parse_anot(self, s, ln, directive):
        res = (s, 0)
        if "+" in s:
            token, off = s.split("+")
            res = (token, int(off))
        if "-" in s:
            token, off = s.split("-")
            res = (token, -int(off))
        if res[0] not in {"pre", "start", "end"}:
            raise ContractParseException(f"line {ln}: invalid annotation token {res[0]} in {directive} directive")
        return res

    def parse_hardware_directive(self, ln, directive, d_args):
        functions = {"opcode": (self.add_hw_opcode, 1),
                     "one-hot": (self.add_hw_one_hot, 3),
                     "variable": (self.add_hw_state_var, 1),
                     "public": (self.add_hw_state_pub, 1),
                     "debug": (self.add_hw_state_debug, 1),
                     "const": (self.add_hw_const, 4),
                     "equiv": (self.add_hw_equiv, 4),
                     "ignore": (self.add_hw_ignore, 1)}
        dir_anot = directive.split("@")

        if len(dir_anot) == 1:
            directive = dir_anot[0]
            if directive in ("one-hot", "const", "equiv"):
                raise ContractParseException(f"line {ln}: missing annotation in {directive} directive")

        if len(dir_anot) == 2:
            directive, anot = dir_anot
            if directive not in ("one-hot", "const", "equiv"):
                raise ContractParseException(f"line {ln}: cannot use annotation in {directive} directive")
            if anot == "":
                raise ContractParseException(f"line {ln}: empty annotation in {directive} directive")
            start = ("pre", 0)
            end = ("end", 0)
            if ":" not in anot:
                start = self._parse_anot(anot, ln, directive)
                end = (start[0], start[1] + 1)
            else:
                s, e = anot.split(":")
                if s != "": start = self._parse_anot(s, ln, directive)
                if e != "": end   = self._parse_anot(e, ln, directive)
            d_args = d_args + [start, end]

        if directive not in functions.keys() or functions[directive][1] != len(d_args):
            raise ContractParseException(f"line {ln}: invalid hardware directive {directive}")
        functions[directive][0](*d_args)

    def parse_memory_directive(self, ln, directive, d_args):
        nargs = {"rdata":  2,
                 "raddr":  2,
                 "req": 1,
                 "gnt": 1,
                 "ack": 1,
                 "we": 1,
                 }
        if directive not in nargs.keys():
            raise ContractParseException(f"line {ln}: unknown memory directive {directive}")
        if len(d_args) != nargs[directive]:
            raise ContractParseException(f"line {ln}: wrong number of parameters for memory directive {directive}")
        # check args: first is hw, then sw
        if d_args[0] not in self.hw_name_ids:
            raise ContractParseException(f"line {ln}: signal {name} does not exist")
        for cname in d_args[1:]:
            if cname not in self.ctrct_state_var:
                raise ContractParseException(f"line {ln}: contract register {cname} does not exist")
        self.state_map_memory[directive] = tuple(d_args)

    def parse_sanity_directive(self, ln, directive, d_args):
        if directive != "hash":
            raise ContractParseException(f"line {ln}: invalid directive in config file: {directive}")
        if len(d_args) != 1:
            raise ContractParseException(f"line {ln}: invalid argument number: {directive}")
        hash_val = d_args[0]
        if hash_val != self.circ_hash:
            raise ContractParseException(f"line {ln}: got hash {hash_val}, expected {self.circ_hash}")

    def parse_directive(self, ln, directive, d_args):
        functions = {"contract": self.parse_contract_directive,
                     "hardware": self.parse_hardware_directive,
                     "mapping": self.parse_mapping_directive,
                     "leaksim": self.parse_leaksim_directive,
                     "memory": self.parse_memory_directive,
                     "sanity": self.parse_sanity_directive}
        if directive not in functions.keys() or len(d_args) < 1:
            raise ContractParseException(f"line {ln}: invalid directive in config file: {directive}")
        sec_dir = d_args[0]
        sec_args = d_args[1:]
        functions[directive](ln, sec_dir, sec_args)

    def parse_config(self, config_path):
        cfg_data = ""  # file contents
        with open(config_path, "r") as f:
            cfg_data = f.read()
        # each line is one stanza, split it accordingly
        cfg_data = cfg_data.strip().split("\n")
        # actual parsing
        ln = 1
        for line in cfg_data:
            # skip empty lines
            line = line.split("//")[0]
            dir_data = line.strip().split()
            if len(dir_data) == 0:
                ln += 1
                continue

            directive = dir_data[0]
            dir_args = dir_data[1:]
            try:
                self.parse_directive(ln, directive, dir_args)
            except ContractParseException as e:
                logging.error(f'failed to parse line {ln}: {line}')
                logging.error('reason: ' + str(e))
                raise # TODO remove
                sys.exit(EXIT_CODE_INPUT_ERR)
            ln += 1
