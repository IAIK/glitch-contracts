import json
import networkx as nx
import functools
import logging
from defines import *
from classes import Cell
import time

CONST_TO_BIT = {"0": 0, "1": 1, "x": -1, "z": -2}

def name_cmp(a, b):
    a_priv, b_priv = a.startswith("_"), b.startswith("_")
    if a_priv and not b_priv: return 1
    if not a_priv and b_priv: return -1
    na = len(a.split(".")) - 1
    nb = len(b.split(".")) - 1
    if na != nb: return na - nb
    return len(a) - len(b)


def bit_to_net(module):
    lookup = {} # cell_num -> {name -> pos}
    for netname in module["netnames"]:
        bits = module["netnames"][netname]["bits"]
        for bit_num, bit, in enumerate(bits):
            if type(bit) is str: continue
            if bit not in lookup:
                lookup[bit] = {}
            lookup[bit][netname] = bit_num
    default = {bit: min(lookup[bit], key=functools.cmp_to_key(name_cmp)) for bit in lookup}
    default = {bit: (default[bit], lookup[bit][default[bit]]) for bit in default}
    return lookup, default


def net_names(module):
    name_bits = {}
    for netname in module["netnames"]:
        bits = module["netnames"][netname]["bits"]
        name_bits[netname] = [x if type(x) is int else CONST_TO_BIT[x] for x in bits]
    return name_bits


class CircuitGraph:
    def __init__(self, circuit_json, top_module):
        self.graph = nx.DiGraph()
        self.circuit_json = circuit_json
        self.top_module = top_module
        self.parse_json()
        #self.write_graph()
        self.print_graph_info()

    def parse_json(self):
        wires = {}  # output -> inputs
        top_module = self.circuit_json["modules"][self.top_module]
        net_names, default_names = bit_to_net(top_module)
        self.graph.clear()

        # Add constants, to make things easier
        # Add nodes and wires for constants 0, 1, x, z
        for name in CONST_TO_BIT.keys():
            self.add_cell(CONST_TO_BIT[name], Cell("const_%s" % name, CONST_TYPE, 0, {("const_%s" % name): 0}))
            wires[CONST_TO_BIT[name]] = [] # the wires which output 0, 1, x, z have NO input

        
        # Add port cells
        for port in top_module["ports"]:
            port_data = top_module["ports"][port]
            if port_data["direction"] == "output": continue 

            # Input ports: 
            # 1) Add node
            # 2) Add wire - the wires which output the input ports have NO input (eg clk_sys of ibex has no input)
            for bit in port_data["bits"]:
                assert(type(bit) == int), "Port bits must be integers"
                defaults = default_names[bit]
                self.add_cell(bit, Cell(defaults[0], PORT_TYPE, defaults[1], net_names[bit]))
                wires[bit] = []

        found_types = set()
        # Gather all cells
        for cell in top_module["cells"]:
            cell_data = top_module["cells"][cell]
            assert(cell_data["type"][:2] == "$_" and cell_data["type"][-1] == "_")
            cell_type_str = cell_data["type"][2:-1].lower()
            cell_type = cell_enum[cell_type_str] # xor, mux, not, or, and, register
            found_types.add(cell_type)
            directions = cell_data["port_directions"]
            connections = cell_data["connections"]

            # Special treatment for mux and dff
            select_bit = None
            mux_in_bits = None
        
            if cell_type == MUX_TYPE:
                select_bit = connections.pop("S")[0]
                if type(select_bit) == str:
                    select_bit = CONST_TO_BIT[select_bit]
            clock_bit = None
            if cell_type in REGISTER_TYPES:
                clock_bit = connections.pop("C")[0]
                
            reset_bit = None
            if cell_type in REGISTER_TYPES and "R" in connections.keys():
                reset_bit = connections.pop("R")[0]

            # Get the cell ports
            connections_sorted = sorted(list(connections.keys()))
            in_port_names = [in_port_name for in_port_name in connections_sorted if directions[in_port_name] == "input"]
            out_port_names = [out_port_name for out_port_name in connections_sorted if directions[out_port_name] == "output"]
            assert(len(out_port_names) == 1)

            # Get the cell wires
            #in_bits = [connections[in_port_name][0] for in_port_name in in_port_names]
            #in_bits = [(CONST_TO_BIT[in_bit] if type(in_bit) == str else in_bit) for in_bit in in_bits]
            in_bits = []
            for in_port_name in in_port_names:
                in_bit = connections[in_port_name][0]
                in_bits.append(CONST_TO_BIT[in_bit] if type(in_bit) == str else in_bit)
            
            out_bits = connections[out_port_names[0]]
            assert(len(out_bits) == 1)
            out_bit = out_bits[0]
            assert(type(out_bit) == int)

            # Add the cell and its wires
            wires[out_bit] = in_bits


            if cell_type == MUX_TYPE: mux_in_bits = in_bits
            defaults = default_names[out_bit]
            cell = Cell(defaults[0], cell_type, defaults[1], net_names[out_bit], select_bit, mux_in_bits, clock_bit, reset_bit)
            self.add_cell(out_bit, cell)

        # sanity check: all registers with reset must have the same reset
        check = lambda x: x in found_types
        #assert(any(map(check, REGISTER_NEG_TYPES)) ^ any(map(check, REGISTER_POS_TYPES))), \
            #"Having both types of register reset is not allowed"

        # Create all connections
        for out_bit in self.graph.nodes():
            # print(out_bit, ", ".join(["%d %d" % (n, n in self.graph.nodes()) for n in wires[out_bit]]))
            assert(all([n in self.graph.nodes() for n in wires[out_bit]]))
            
            for in_wire in wires[out_bit]:
                self.graph.add_edge(in_wire, out_bit)

    def add_cell(self, bit, cell):
        assert(bit not in self.graph.nodes())
        self.graph.add_node(bit, **{"cell": cell})

    def print_graph_info(self):
        num_regs = len([n for n in self.graph.nodes() if self.graph.nodes[n]["cell"].type in REGISTER_TYPES])
        num_lin = len([n for n in self.graph.nodes() if self.graph.nodes[n]["cell"].type in LINEAR_TYPES])
        num_nonlin = len([n for n in self.graph.nodes() if self.graph.nodes[n]["cell"].type in NONLINEAR_TYPES])
        num_muxs = len([n for n in self.graph.nodes() if self.graph.nodes[n]["cell"].type == MUX_TYPE])
        total = len(list(self.graph.nodes()))
        logging.info("| CircuitGraph | Total: %4d | Linear: %4d | Non-linear: %4d | Registers: %4d | Mux: %4d | " %
              (total, num_lin, num_nonlin, num_regs, num_muxs))

    def write_graph(self):
        dot = "strict digraph  {\n"
        for e in self.graph.edges():
            src_cell = self.graph.nodes[e[0]]["cell"]
            dst_cell = self.graph.nodes[e[1]]["cell"]
            #dot += "\"%s\" -> \"%s\";\n" % (src_cell.name, dst_cell.name)
            types = [inv_cell_enum[x.type] for x in (src_cell, dst_cell)]
            src_str = "\"%s_%s_%d\"" % (types[0], src_cell.name, e[0])
            dst_str = "\"%s_%s_%d\"" % (types[1], dst_cell.name, e[1])
            dot += "%s -> %s;\n" % (src_str, dst_str)
        dot += "}\n"
        with open(TMP_DIR + "/circuit.dot", "w") as f:
            f.write(dot)

    def write_pickle(self, args):
        t1 = time.time()
        nx.write_gpickle(self.graph, args.circuit_graph_path) #TMP_DIR + "/circuit_graph.gpickle") 
        t2 = time.time()
        logging.info("Writing CircuitGraph: %.2f" % (t2-t1))
