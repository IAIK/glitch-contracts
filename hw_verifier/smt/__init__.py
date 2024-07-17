
import logging
import time
import traceback
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Any, Union

#import z3
from .pytape import z3
from .expr import *

sat = 10
unsat = 20
unknown = 30

set_option = z3.set_option
    

def create_solver(solver: str, theories: str):
    match solver:
        case "z3":
            from .z3solver import Z3Solver
            return Z3Solver(theories)
        case "btor":
            from .boolector import BoolectorSolver
            return BoolectorSolver(theories)
        case _:
            raise Exception('solver not supported')
