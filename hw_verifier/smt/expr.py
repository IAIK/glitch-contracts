from __future__ import annotations

import traceback
from dataclasses import dataclass
from typing import Any, Union

#import z3
from .pytape import z3

#exprclass = dataclass(eq=False, frozen=True, unsafe_hash=True)
exprclass = dataclass(eq=True, frozen=False, unsafe_hash=True)  # TODO TODO TODO TODO TODO TODO TODO TODO freeze

def to_z3(exp: Expr) -> z3.ExprRef:
    match exp:
        case Not(e):                return z3.Not(to_z3(e))
        case _And(ch):              return z3.And(*(to_z3(x) for x in ch))
        case _Or(ch):               return z3.Or(*(to_z3(x) for x in ch))
        case _Xor(ch):              return z3.Xor(*(to_z3(x) for x in ch))
        case Implies(a, b):         return z3.Implies(to_z3(a), to_z3(b))
        case If(c, t, e):           return z3.If(to_z3(c), to_z3(t), to_z3(e))
        case BoolVal(value):        return z3.BoolVal(value)
        case Bool(name):            return z3.Bool(name)
        case BitVecVal(value, len): return z3.BitVecVal(value, len)
        case BitVec(name, len):     return z3.BitVec(name, len)
        case Extract(s, e, bv):     return z3.Extract(s, e, to_z3(bv))
        case Equals(a, b):          return to_z3(a) == to_z3(b)
        case ExprWrapper(a):        return a
        case _AtMost(children, n):  return z3.AtMost(*children, n) # TODO check
        case _:                     raise Exception(f'not implemented: {exp}')

@exprclass
class Expr:
    def eq(self, other: Union[bool, int, Expr]) -> bool:
        ''' always use z3 to check for AST equality'''
        return to_z3(self).eq(to_z3(other))

@exprclass
class Not(Expr):
    a: Expr
    cashed_hash = None
    
    def __str__(self):
        return f'(not {self.a})'

@exprclass
class _And(Expr):
    children: list[Expr]
    cashed_hash = None
    
    def __str__(self):
        return '(' + ' and '.join(str(child) for child in self.children) +  ')' 

@exprclass
class _Or(Expr):
    children: list[Expr]
    cashed_hash = None
    
    def __str__(self):
        return '(' + ' or '.join(str(child) for child in self.children) +  ')' 

@exprclass
class _Xor(Expr):
    children: list[Expr]
    cashed_hash = None
    
    def __str__(self):
        return '(' + ' xor '.join(str(child) for child in self.children) +  ')' 

@exprclass        
class Implies(Expr):
    a: Expr
    b: Expr
    cashed_hash = None
    
    def __str__(self):
        return f'({self.a} -> {self.b})'

@exprclass
class If(Expr):
    cond: Expr
    thenBranch: Expr
    elseBranch: Expr
    cashed_hash = None
    
    def __str__(self):
        return f'({self.cond} ? {self.thenBranch} :  {self.elseBranch} )'

@exprclass
class BoolVal(Expr):
    value: bool
    cashed_hash = None
    
    def __str__(self):
        return str(self.value)

@exprclass
class Bool(Expr):
    name: str
    exists: bool = False
    cashed_hash = None
    
    def __str__(self):
        return str(self.name)

@exprclass
class BitVecVal(Expr):
    value: bool
    len: int
    cashed_hash = None
    
    def __str__(self):
        return f'0b{self.value:0{self.len}b}'

@exprclass
class BitVec(Expr):
    name: str
    len: int
    exists: bool = False
    cashed_hash = None
    
    def __str__(self):
        return str(self.name)

@exprclass
class Extract(Expr):
    start: Expr
    end: Expr
    bv: Expr
    cashed_hash = None
    
    def __str__(self):
        return f'{self.bv}[{self.end}:{self.start}]'

@exprclass
class Equals(Expr):
    a: Expr
    b: Expr
    cashed_hash = None
    
    def __str__(self):
        return f'({self.a} == {self.b})'

@exprclass        
class _AtMost(Expr):
    n: int
    children: list[Expr]
    cashed_hash = None
    
    def __str__(self):
        return f'AtMost({self.n}, ' + ', '.join(str(child) for child in self.children) + ')'

@exprclass
class ExprWrapper(Expr):
    e: Any
    cashed_hash = None
    
    def __str__(self):
        return 'wrapped(' + str(self.e) +  ')' 

        
def And(*children):
    assert all(isinstance(x, Expr) for x in children)
    return _And(children)


def Or(*children):
    assert all(isinstance(x, Expr) for x in children)
    return _Or(children)


def Xor(*children):
    assert all(isinstance(x, Expr) for x in children)
    return _Xor(children)


def AtMost(*args):
    children = args[:-1]
    n = args[-1]
    return _AtMost(n, children)
        
        
for cls in [Not, _And, _Or, _Xor, Implies, If, BoolVal, Bool, BitVecVal, BitVec, Extract, Equals, _AtMost, ExprWrapper]:
    default_hash_Bool = cls.__hash__
    def cached__hash__(self, dh=default_hash_Bool):
        if self.cashed_hash is None:
            res = dh(self)
            self.cashed_hash = res
        return self.cashed_hash
    cls.__hash__ = cached__hash__
