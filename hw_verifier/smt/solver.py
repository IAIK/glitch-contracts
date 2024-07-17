from abc import ABC, abstractmethod
from dataclasses import dataclass

from .expr import Expr, Bool, BitVec
from . import sat, unsat
import time

class Solver(ABC):
    def __init__(self):
        self.expr_cache = {}
        self.type_assignments = {}
        self.dbg_assumes = []
        
    def check_type_assignment(self, exp):
        name = None
        len = None
        #if not isinstance(exp, Expr):
        #    raise Exception(f'Cannot encode non-Expr type {type(exp)} (value = {exp})')
        match exp:
            case Bool(n):      name = n; len = -1
            case BitVec(n, l): name = n; len = l
        if name is not None:
            type_assignment = self.type_assignments.get(name)
            if type_assignment:
                assert type_assignment == len
            else:
                self.type_assignments[name] = len

    def encode(self, exp: Expr, use_cache: bool = True):
        self.check_type_assignment(exp)
        if not use_cache:
            return self.encode_internal(exp)
        try:
            cached_exp = self.expr_cache.get(exp)
        except TypeError:
            raise Exception(f'Invalid expression {exp}')
        if cached_exp is not None:
            return cached_exp
        res = self.encode_internal(exp)
        self.expr_cache[exp] = res
        return res
    
    def check(self, *assumptions, **kwargs) -> int:
        #start = time.time()
        res = self._check(*assumptions, **kwargs)
        #end = time.time()
        #print()
        #print('total check time', end-start, self.dbg_assumes, flush=True)
        #print()
        assert res in [sat, unsat]
        return res
    
    def dbg_learn(self, exp):
        self.dbg_assumes.append(exp)
    
    def dbgdump(self):
        with open(self.dbgname, 'w') as f:
            for ass in self.dbg_assumes:
                f.write(str(ass)+'\n')
                
    @abstractmethod
    def encode_internal(self, exp: Expr):
        ...
        
    @abstractmethod
    def _check(self, *assumptions, **kwargs):
        ...
            