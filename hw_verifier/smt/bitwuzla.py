import logging
import time
import traceback
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Any, Union

import pybitwuzla

#class BitwuzlaSolver(Solver):
#    def __init__(self, theories):
#        super().__init__(theories)
#        self.var_cache = {}
#        self.expr_cache = {}
#        self.bvsort_cache = {}
#        self.level = 0
#        self.debug_time = 0
#        if theories != 'QF_BV':
#            raise Exception('better use z3')
#        
#        self.solver = pybitwuzla.Bitwuzla()
#        self.solver.set_option(pybitwuzla.Option.INCREMENTAL, 1)
#        #self.solver.set_option(pybitwuzla.Option.MODEL_GEN, 1) # TODO make optional
#         
#    def bvsort(self, len):
#        if len not in self.bvsort_cache:
#            self.bvsort_cache[len] = self.solver.BitVecSort(len)
#        return self.bvsort_cache[len]
#        
#    def var(self, len, name, exists):
#        if exists:
#            try:
#                cached_var = self.solver.Match_by_symbol(f'|{name}|')
#            except pyboolector.BoolectorException:
#                print('could not get var', name, len)
#                cached_var = self.solver.Match_by_symbol(f'{name}')
#            return cached_var | self.solver.Const(0, len)
#        # TODO ensure not exists
#        if (len, name) not in self.var_cache:
#            self.var_cache[(len, name)] = self.solver.Var(self.bvsort(len), name)
#        return self.var_cache[(len, name)]
#        
#    def encode(self, exp: Expr):
#        cached_exp = self.expr_cache.get(exp)
#        if cached_exp is not None:
#            return cached_exp
#        s = self.solver
#        # TODO XOR > 3 wrong if (type(exp) == )
#        match exp:
#            case Not(e):               res = s.Not(self.encode(e))
#            case _And([e]):            res = self.encode(e)
#            case _Or([e]):             res = self.encode(e)
#            case _Xor([e]):            res = self.encode(e)
#            case _And(ch):             res = s.And(self.encode(ch[0]), self.encode(_And(ch[1:]))) 
#            case _Or(ch):              res = s.Or(self.encode(ch[0]), self.encode(_Or(ch[1:])))
#            case _Xor(ch):             res = s.Xor(self.encode(ch[0]), self.encode(_Xor(ch[1:])))
#            case Implies(a, b):        res = s.Implies(self.encode(a), self.encode(b))
#            case If(c, t, e):          res = s.Cond(self.encode(c), self.encode(t), self.encode(e))
#            case BoolVal(value):       res = s.Const(value)
#            case Bool(name, exists):   res = self.var(1, name, exists=exists) # TODO check if this is best
#            case BitVecVal(value, le): res = s.Const(value, le)
#            case BitVec(name, le, exists):res = self.var(le, name, exists=exists)
#            case Extract(h, l, bv):    res = s.Slice(self.encode(bv), h, l) # TODO order?
#            case Equals(a, b):         res = s.Iff(self.encode(a), self.encode(b))
#            #case Equals(a, b):         res = self.encode(a) == self.encode(b)
#            case _AtMost(children, n): raise Exception(f'not implemented: {exp}') #res = z3.AtMost(*children, n) # TODO check
#            case _: raise Exception(f'not implemented: {exp}')
#        self.expr_cache[exp] = res
#        return res
#    
#    def num_scopes(self):
#        return self.level
#    
#    def push(self):
#        self.level += 1
#        return self.solver.Push()
#    
#    def pop(self):
#        self.level -= 1
#        return self.solver.Pop()
#    
#    def add(self, exp: Expr):
#        encoded_exp = self.encode(exp)
#        self.solver.Assert(encoded_exp)
#    
#    def check(self, *assumptions) -> z3.CheckSatResult:
#        if len(assumptions) > 0 and type(assumptions[0]) == list:
#            assert len(assumptions) == 1
#            assumptions = assumptions[0]
#        assumptions = (self.encode(x) for x in assumptions)
#        for ass in assumptions:
#            self.solver.Assume(ass)
#        return self.solver.Sat()
#
#    def reset(self):
#        del self
#            
#    def model(self):
#        class Model:
#            def __init__(self, solver):
#                self.solver = solver
#            def eval(self, var, model_completion=False):
#                class assignment:
#                    def __init__(self, val):
#                        self.val = val
#                    def sexpr(self):
#                        return self.val
#                return assignment(self.solver.encode(var).assignment)
#        return Model(self)
#    
#    def from_file(self, filename: str):
#        result, status, error_msg = self.solver.Parse(filename)
#        assert error_msg == None
#        assert status == pyboolector.Boolector.UNKNOWN
#        assert result != pyboolector.Boolector.PARSE_ERROR
#    
#    def to_smt2(self):
#        #from pathlib import Path
#        #Path('tmpfile').unlink()
#        #self.solver.Print_model('smt2', 'tmpfile')
#        #with open('tmpfile', 'r') as f: # TODO fix
#        #    return f.read()
#        return "not implemented" # TODO
#        return self.solver.to_smt2()
#    
#    def dbgdump(self):
#        with open(self.dbgname, 'w') as f:
#            for ass in self.dbg_assumes:
#                f.write(str(ass)+'\n')
