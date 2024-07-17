import logging
import os
import tempfile
import time
import traceback
from abc import ABC, abstractmethod
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Union

import pyboolector

from . import sat, unknown, unsat
from .expr import *
from .expr import _And, _AtMost, _Or, _Xor
from .solver import Solver


class BoolectorSolver(Solver):
    def __init__(self, theories):
        super().__init__()
        self.const_cache = {}
        self.var_cache = {}
        self.bvsort_cache = {}
        self.level = 0
        self.debug_time = 0
        self.assumptions = []
        if theories != 'QF_BV':
            raise Exception('better use z3')
        
        self.solver = pyboolector.Boolector()
        self.solver.Set_opt(pyboolector.BTOR_OPT_INCREMENTAL, True)
        #self.solver.Set_opt(pyboolector.BTOR_OPT_SAT_ENGINE, pyboolector.BTOR_SAT_ENGINE_CMS)
        #self.solver.Set_opt(pyboolector.BTOR_OPT_ENGINE, pyboolector.BTOR_ENGINE_SLS)
        #self.solver.Set_opt(pyboolector.BTOR_OPT_INCREMENTAL_SMT1, pyboolector.BTOR_INCREMENTAL_SMT1_CONTINUE)
        self.solver.Set_opt(pyboolector.BTOR_OPT_MODEL_GEN, True)
        #self.solver.Set_opt(pyboolector.BTOR_OPT_SAT_ENGINE, pyboolector.BTOR_SAT_ENGINE_CMS)
        #self.solver.Set_opt(pyboolector.BTOR_OPT_FUN_JUST, True)
        

        self.BoolSort = self.solver.BoolSort()
        
    def bvsort(self, len):
        if len not in self.bvsort_cache:
            self.bvsort_cache[len] = self.solver.BitVecSort(len)
        return self.bvsort_cache[len]
    
    def const(self, val: int, len: int = 1):
        if len not in self.const_cache:
            self.const_cache[(val, len)] = self.solver.Const(val, len)
        return self.const_cache[(val, len)]
        
    def var(self, len, name, exists):
        if exists:
            try:
                cached_var = self.solver.Match_by_symbol(f'|{name}|')
            except pyboolector.BoolectorException:
                raise Exception(f'var does not exist: "{name}" (length {len})')
            return cached_var | self.const(0, len)
        # TODO ensure not exists
        key = (len, name)
        cached_var = self.var_cache.get(key)
        if cached_var is None:
            cached_var = self.solver.Var(self.bvsort(len), name)
            self.var_cache[key] = cached_var
        return cached_var
        
    def encode_internal(self, exp: Expr):
        s = self.solver
        # TODO XOR > 3 wrong if (type(exp) == )
        match exp:
            case Not(e):                   res = s.Not(self.encode(e))
            case _And([e]):                res = self.encode(e)
            case _Or([e]):                 res = self.encode(e)
            case _Xor([e]):                res = self.encode(e)
            case _And(ch):                 res = s.And(self.encode(ch[0]), self.encode(_And(ch[1:]))) 
            case _Or(ch):                  res = s.Or(self.encode(ch[0]), self.encode(_Or(ch[1:])))
            case _Xor(ch):                 res = s.Xor(self.encode(ch[0]), self.encode(_Xor(ch[1:])))
            case Implies(a, b):            res = s.Implies(self.encode(a), self.encode(b))
            case If(c, t, e):              res = s.Cond(self.encode(c), self.encode(t), self.encode(e))
            case BoolVal(value):           res = self.const(value)
            case Bool(name, exists):       res = self.var(1, name, exists=exists) # TODO check if this is best
            case BitVecVal(value, le):     res = self.const(value, le)
            case BitVec(name, le, exists): res = self.var(le, name, exists=exists)
            case Extract(h, l, bv):        res = s.Slice(self.encode(bv), h, l)
            case Equals(a, b):             res = s.Eq(self.encode(a), self.encode(b))
            case ExprWrapper(a):           res = a
            case _AtMost(n, ch):
                w = len(ch).bit_length()
                res = s.Ult(sum(s.Uext(self.encode(x), w-1) for x in ch), self.const(n, w))
            case _:                        raise Exception(f'not implemented: {exp}')
        return res
    
    def num_scopes(self):
        return self.level
    
    def push(self):
        self.level += 1
        return self.solver.Push()
    
    def pop(self):
        self.level -= 1
        return self.solver.Pop()
    
    def add(self, exp: Expr):
        encoded_exp = self.encode(exp)
        self.solver.Assert(encoded_exp)
    
    def _check(self, *assumptions, modelgen=True) -> int:
        if len(assumptions) > 0 and type(assumptions[0]) == list:
            assert len(assumptions) == 1
            assumptions = assumptions[0]
        self.assumptions = assumptions
        enc_assumptions = [self.encode(x) for x in assumptions]
        for ass in enc_assumptions:
            self.solver.Assume(ass)

        if not modelgen:
            self.solver.Set_opt(pyboolector.BTOR_OPT_MODEL_GEN, False)
        res = self.solver.Sat()
        if not modelgen:
            self.solver.Set_opt(pyboolector.BTOR_OPT_MODEL_GEN, True)
        return res

    def reset(self):
        del self
            
    def model(self):
        class Model:
            def __init__(self, solver):
                self.solver = solver
                
            def eval(self, exp: Expr, model_completion=False):
                class assignment:
                    def __init__(self, val):
                        self.val = val
                    def sexpr(self):
                        return self.val
                return assignment(self.solver.encode(exp).assignment)
            
            def __str__(self):
                with tempfile.TemporaryDirectory(prefix='pyboolector_') as tempdir:
                    fname = os.path.join(tempdir, 'model.smt')
                    self.solver.solver.Print_model('smt2', fname)
                    with open(fname, 'r') as f:
                        return f.read()
            
            def evalBool(self, exp: Expr):
                return self.solver.encode(exp).assignment == '1'

        return Model(self)
    
    def dbg_to_str(x):
        ...

    def _minimize_recursive_fixed(self, needed: list[Expr], curr_slice: list[Expr],
                                  rest: list[Expr], depth=0, part=None, assume_nonempty=False):
        
        print(self.dbg_to_str(needed), 
        '|', self.dbg_to_str(curr_slice), 
        '|', self.dbg_to_str(rest))
        
        if len(curr_slice) == 0:
            return needed + rest

        if not assume_nonempty:
            res = self.check(needed + rest, modelgen=False)
            print(self.dbg_to_str(needed + rest), '->', res)
            if res == unsat:
                fails = self.solver.Failed(*(self.encode(x) for x in self.assumptions))
                min_core = [ass for ass, failed in zip(self.assumptions, fails) if failed]
                print('newcore', self.dbg_to_str(min_core))
                return min_core
            else:
                if len(curr_slice) == 1:
                    #needed += curr_slice
                    return needed + curr_slice + rest

        part1, part2 = curr_slice[:len(curr_slice)//2], curr_slice[len(curr_slice)//2:]
        p1_core = self._minimize_recursive_fixed(needed, part1, part2+rest, depth+1, 'part1', assume_nonempty=False)
        part1_needed = [x for x in p1_core if x in part1]
        needed += part1_needed
        assert all(x in p1_core for x in needed), f'{needed} not in {p1_core}'
        p1empty = len(part1_needed) == 0
        part2 = [x for x in part2 if x in p1_core]
        rest = [x for x in rest if x in p1_core]
        p2_remaining = self._minimize_recursive_fixed(needed, part2, rest, depth+1, 'part2', assume_nonempty=p1empty)
        return p2_remaining
    
    def _minimize_recursive(self, needed: list[Expr], curr_slice: list[Expr], rest: list[Expr], depth=0, part=None):
        part1, part2 = curr_slice[:len(curr_slice)//2], curr_slice[len(curr_slice)//2:]
        
        # process part 1
        res = self.check(needed + part2 + rest)
        if res == unsat:
            fails = self.solver.Failed(*(self.encode(x) for x in self.assumptions))
            min_core = [ass for ass, failed in zip(self.assumptions, fails) if failed]
            rest[:] = [x for x in min_core if x not in needed]
        else:
            if len(part1) <= 1:
                needed += part1
            else:
                self._minimize_recursive(needed, part1, part2 + rest, depth+1, 'part1')
        
        # process part 2
        if len(part2) == 0:
            return
        if len(part2) == 1:
            res = self.check(needed + rest)
            if res == unsat:
                fails = self.solver.Failed(*(self.encode(x) for x in self.assumptions))
                min_core = [ass for ass, failed in zip(self.assumptions, fails) if failed]
                rest[:] = [x for x in min_core if x not in needed]
            else:
                needed += part2
            return
        self._minimize_recursive(needed, part2, rest, depth+1, 'part2')
    
    def minimize_recursive(self, core: list[Expr]):
        rest = []
        curr_slice = core[:]
        min_core = []
        
        def debug_to_str(li):
            return ' '.join(str(core.index(x)) for x in li)
            
        self.dbg_to_str = debug_to_str
        
        self._minimize_recursive_fixed(min_core, curr_slice, rest)
        print(f'core minimized from size {len(core)} to {len(min_core)}')
        return min_core
    
    def minimize_core_faster(self, core: list[Expr]):
        
        def n_parts(li: list, n: int):
            if n >= len(li):
                return [[x] for x in li]
            siz = len(li) / n
            parts = []
            start = 0
            for i in range(n):
                end = int((i+1)*siz)
                if i == n - 1:
                    end = -1
                parts.append(li[start:end])
                start = end
            res = [li[int(i*siz):int((i+1)*siz)] for i in range(n)]
            assert len(li) == sum(len(x) for x in res)
            assert set(li) == set(y for x in res for y in x)
            return res
         
        tried = []
        min_core = core[:]
        numparts = 2
        while True:
            parts = n_parts(min_core, numparts)
            for part in parts:
                test_core = [x for x in min_core if x not in part]
                for tried_part in tried:
                    if all(x not in test_core for x in tried_part):
                        res = sat
                        break
                else:
                    res = self.check(test_core)
                if res == unsat:
                    fails = self.solver.Failed(*(self.encode(x) for x in self.assumptions))
                    min_core = [ass for ass, failed in zip(self.assumptions, fails) if failed]
                else:
                    tried.append(part)
            if numparts >= len(min_core):
                break
            numparts *= 2
            if len(min_core) < numparts * 4:
                numparts = len(min_core)
        print(f'core minimized from size {len(core)} to {len(min_core)}')
        return min_core
    
    def minimize_core(self, core: list[Expr]):
        tried = []
        min_core = core[:]
        numparts = 2
        while True:
            not_tried = [x for x in min_core if x not in tried]
            if len(not_tried) == 0:
                break
            cur_elem = not_tried[0] # TODO better
            curr_core = min_core[:]
            curr_core.remove(cur_elem)
            tried.append(cur_elem)
            res = self.check(curr_core, modelgen=False)
            if res == unsat:
                fails = self.solver.Failed(*(self.encode(x) for x in self.assumptions))
                min_core = [ass for ass, failed in zip(self.assumptions, fails) if failed]
        print(f'core minimized from size {len(core)} to {len(min_core)}')
        return min_core
    
    def unsat_core(self, minimize=True):
        class Core:
            def __init__(self, solver, core):
                self.solver = solver
                self.core = core
            def __contains__(self, item):
                return item in self.core
            def __iter__(self):
                return (x for x in self.core)
        fails = self.solver.Failed(*(self.encode(x) for x in self.assumptions))
        core = [ass for ass, failed in zip(self.assumptions, fails) if failed]
        if minimize:
            mcore = self.minimize_core_faster(core)
            core = mcore
        return Core(self, core)
    
    def from_file(self, filename: str):
        result, status, error_msg = self.solver.Parse(filename)
        assert error_msg == None, f'CBMC Error: {error_msg}'
        assert status == pyboolector.Boolector.UNKNOWN
        assert result != pyboolector.Boolector.PARSE_ERROR
    
    def to_smt2(self):
        return "not implemented"

    def to_str(self, exp):
        from pathlib import Path
        Path('tmpfile').unlink()
        exp.Dump('smt2', 'tmpfile')
        with open('tmpfile', 'r') as f:
            return f.read()

    def simplify(self):
        self.solver.Simplify()
