import os

REGISTER_NAMES = ("dff_p", "dff_pp0", "dff_pp1", "dff_pn0", "dff_pn1")
REGISTER_TYPES = range(0, 5)
REGISTER_POS_TYPES = range(1, 3)
REGISTER_NEG_TYPES = range(3, 5)
REGISTER_TYPE_RESET_VAL_0 = (1, 3)
REGISTER_TYPE_RESET_VAL_1 = (2, 4)

PORT_NAME = "port"
PORT_TYPE = 5

REGPORT_TYPES = range(0, 6)

LINEAR_NAMES = ("xor", "xnor")
XOR_TYPE, XNOR_TYPE = 6, 7
LINEAR_TYPES = range(6, 8)

NONLINEAR_NAMES = ("and", "or")
AND_TYPE, OR_TYPE = 8, 9
NONLINEAR_TYPES = range(8, 10)

GATE_NAMES = LINEAR_NAMES + NONLINEAR_NAMES
GATE_TYPES = range(6, 10)

OTHER_NAMES = ("mux", "const", "not")
MUX_TYPE, CONST_TYPE, NOT_TYPE = 10, 11, 12
OTHER_TYPES = range(10, 13)

# define the cell type enum and strings
cell_enum = {}
name_val_pairs = ((REGISTER_NAMES, REGISTER_TYPES),
                  (GATE_NAMES, GATE_TYPES),
                  (OTHER_NAMES, OTHER_TYPES))

for names, vals in name_val_pairs:
    for name, val in zip(names, vals):
        cell_enum[name] = val
cell_enum[PORT_NAME] = PORT_TYPE

inv_cell_enum = {cell_enum[x]: x for x in cell_enum}

LIST_XOR = "shallow"

TRIGGERS = {AND_TYPE: "0", OR_TYPE: "1", XOR_TYPE: None, XNOR_TYPE: None}
BIN_STR = ("0", "1")
UINT_MAX = 0xffffffffff

# define commonly used keys
TRANSIENT = "transient" # TODO rename to transition
STABLE = "stable"
GLITCH = "glitch"

# define label types
LABEL_VARIABLE = "var"
LABEL_CONST = "const"
LABEL_TYPES = (LABEL_VARIABLE, LABEL_CONST)

# directory defines
SRC_DIR = os.path.dirname(os.path.realpath(__file__))
TEMPLATE_DIR = SRC_DIR + "/templates/"
TMP_DIR = SRC_DIR + "/tmp"
ROOT_DIR = "/".join(SRC_DIR.split("/")[:-1])

SUFFIXES = ("_A", "_nA")

# exit codes for contract compliance verification
EXIT_CODE_COMPLIANT    =  0
EXIT_CODE_NONCOMPLIANT = -1
EXIT_CODE_INPUT_ERR    = -2
EXIT_CODE_UNREACHABLE  = -3
EXIT_CODE_TODO         = -4

Z3_TYPE_BOOL   = 0
Z3_TYPE_BITVEC = 1

NUM_CONTEXTS = 2
