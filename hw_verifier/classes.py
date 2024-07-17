from dataclasses import dataclass
from defines import *

from typing import Optional

@dataclass
class SignalConstraint:
    start: tuple[str, int]   # inclusive
    end:   tuple[str, int]   # exclusive
    args:  tuple        # list of arguments

    def __hash__(self):
        return hash((self.start, self.end, *self.args))

    @staticmethod
    def decode(token, offset, last) -> int:
        # return offset + ({"pre": -1, "start": 0, "end": last + 1}[token])
        if token == "pre":
            return -1 + offset
        elif token == "start":
            return 0 + offset
        elif token == "end":
            return last + 1 + offset
        raise Exception(f'Unexpected token {token}')
        #assert(False)

    def in_range(self, curr, last):
        start = SignalConstraint.decode(*self.start, last)
        end = SignalConstraint.decode(*self.end, last)
        return start <= curr < end


@dataclass
class Cell:
    name: str
    type: int
    pos: int
    names: dict
    select: Optional[int] = None
    mux_ins: Optional[list] = None
    clock: Optional[int] = None
    reset: Optional[int] = None

    def __lt__(self, other):
        return str(self.name) < str(other.name)

    def __str__(self):
            return "Type: %s | Name: %s | Pos: %d" % (inv_cell_enum[self.type], self.name, self.pos)


@dataclass
class CfgZ3Typ:
    z3_ty  : int
    length : int

    def __init__(self, decl):
        decl = decl.strip()
        if decl.startswith("BitVec"):
            ty, tlen = decl.split(" ", 1)
            if not " " in tlen.strip():
                self.z3_ty = Z3_TYPE_BITVEC
                self.length = int(tlen.strip())
            else:
                print(f"cannot parse z3 type declaration {decl}")
                assert False
        elif decl.startswith("Bool"):
            self.z3_ty = Z3_TYPE_BOOL
            self.length = 1
        else:
            print(f"cannot parse z3 type declaration {decl}")
            assert False
