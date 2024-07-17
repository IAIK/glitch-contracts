import logging
import time
import traceback
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Any, Union

import sys

#import z3
from .pytape import z3

from . import sat, unknown, unsat
from .expr import *
from .expr import _And, _Or, _Xor, _AtMost
from .solver import Solver

def res_from_z3(res: z3.CheckSatResult) -> int:
    match res:
        case z3.sat: return sat
        case z3.unsat: return unsat
        case z3.unknown: return unknown
        case _: raise Exception(f'unexpected z3 solver result {res}')


class Z3Solver(Solver):
    def __init__(self, theories: str):
        super().__init__()
        self.dbgctr = 0
        self.debug_time = 0
        #if theories == 'QF_BV':
        #    self.solver = z3.Then('simplify', 'solve-eqs', 'qfbv').solver()
        #else:
        #    self.solver = z3.SolverFor(theories)
        self.solver = z3.SolverFor(theories) # logFile='z3dump.smt'
        self.solver.set("smt.core.minimize", True)
        
        z3.set_option(max_args  = 10000000, max_lines   = 1000000,
                      max_depth = 10000000, max_visited = 1000000) # TODO move to lib
        #self.solver.set(unsat_core=True)
        
    def encode_internal(self, exp: Expr):
        match exp:
            case Not(e):                res = z3.Not(self.encode(e))
            case _And(ch):              res = z3.And(*(self.encode(x) for x in ch))
            case _Or(ch):               res = z3.Or(*(self.encode(x) for x in ch))
            case _Xor(ch):              res = z3.Xor(*(self.encode(x) for x in ch))
            case Implies(a, b):         res = z3.Implies(self.encode(a), self.encode(b))
            case If(c, t, e):           res = z3.If(self.encode(c), self.encode(t), self.encode(e))
            case BoolVal(value):        res = z3.BoolVal(value)
            case Bool(name):            res = z3.Bool(name)
            case BitVecVal(value, len): res = z3.BitVecVal(value, len)
            case BitVec(name, len):     res = z3.BitVec(name, len)
            case Extract(s, e, bv):     res = z3.Extract(s, e, self.encode(bv))
            case Equals(a, b):          res = self.encode(a) == self.encode(b)
            case ExprWrapper(a):        res = a
            case _AtMost(n, ch):        res = z3.AtMost(*(self.encode(x) for x in ch), n) # TODO check
            case _:                     raise Exception(f'not implemented: {exp}')
        return res
    
    def num_scopes(self) -> int:
        return self.solver.num_scopes()
    
    def push(self):
        return self.solver.push()
    
    def pop(self):
        return self.solver.pop()
    
    def add(self, exp: Expr):
        z3exp = self.encode(exp)
        self.solver.add(z3exp)
    
    def _check(self, *assumptions, modelgen=True) -> int:
        if len(assumptions) > 0 and type(assumptions[0]) == list:
            assert len(assumptions) == 1
            assumptions = assumptions[0]
        if len(assumptions) > 0 and isinstance(assumptions[0], Expr): # TODO remove conditions
            assumptions = (self.encode(x) for x in assumptions)
        res = self.solver.check(*assumptions)
        return res_from_z3(res)
    
    def reset(self):
        self.solver.reset()
        
    def check_model_consistency(self):
        return
        model = self.solver.model()
        #with tempfile.TemporaryFile() as f
        
    def model(self):
        class Model:
            def __init__(self, solver, model):
                self.solver = solver
                self.model = model
                
            def __str__(self, ):
                return str(self.model)

            def eval(self, exp: Expr, model_completion=False):
                model_completion=True  # TODO REMOVE
                return self.model.eval(self.solver.encode(exp), model_completion=model_completion)
            
            def evalBool(self, exp: Expr):
                z3exp = self.solver.encode(exp)
                assert z3exp.sort() == z3.BoolSort()
                expstr = self.model.eval(z3exp, model_completion=True).sexpr()
                assert expstr in ['true', 'false'], f'got invalid bool during model evaluation: {expstr}'
                return expstr == 'true'
            
            def evalBools(self, exps: list[Expr]):
                return {x: self.evalBool(x) for x in exps}

        self.check_model_consistency()
        return Model(self, self.solver.model())
    
    def unsat_core(self):
        class Core:
            def __init__(self, solver, core):
                self.solver = solver
                self.core = core
                
            def __contains__(self, item):
                return self.solver.encode(item) in self.core
            
            def __iter__(self):
                return (ExprWrapper(x) for x in self.core)
            
        return Core(self, self.solver.unsat_core()) 
    
    def from_file(self, filename: str):
        self.solver.from_file(filename)
    
    def to_smt2(self):
        return self.solver.to_smt2()

    def simplify(self):
        pass
