#from pysmt.smtlib.parser import SmtLibParser
from pathlib import Path
import re
import pprint
import ast
import sys
from dataclasses import dataclass
from typing import Tuple, Optional, Dict

pp = pprint.PrettyPrinter(width=100, compact=True)

VAL_INJECTOR_REGEX = re.compile(r'nondet_symex::nondet[\d]+')
VAL_TRANSFER_REGEX = re.compile(r'.*return_value_initial_val_helper.*')
VAL_INJECT_REGEX   = re.compile(r'\(define-fun \|([\w#_]*)\| \(\) \([_ \w]*\) \|nondet_symex::nondet[\d]+')
DEF_EQUAL_REGEX    = re.compile(r'\(define-fun \|(.*)\| \(\) \([_ \w]*\) \|(.*)\|\)')
LEAK_VAL_REGEX     = re.compile(r'\(define-fun \|([\w#_]*)\| \(\) \([_ \w]*\) \|leak\((unsigned_int,)*unsigned_int\)::e(\d+)!(\d+)@(\d+)#(\d+)(\.\.\S+)\|')
LEAK_GUARD_REGEX   = re.compile(r'\(define-fun \|([\w#_]*)\| \(\) \([_ \w]*\) \(ite \|([\w#_:/]*)\| \|([\w#_]*)\| \|([\w#_]*)\|')
LOG_EQUALITY_REGEX = re.compile(r'\([0-9]+\) ([^ ]+) == ([^ ]+)')
LEAK_NAME_REGEX    = re.compile(r'__leak_f([0-9]+)_e([0-9]+)')
BOOL_FILTER1       = re.compile(r'\(define-fun B[0-9]+ \(\) Bool .*\)')
BOOL_FILTER2       = re.compile(r'\(assert B[0-9]+\)')
STRUCT_DECL_FILTER = re.compile(r'^\(declare-fun .* (.*) struct.\d+\)$')
TAUTOLOGY_FILTER1  = re.compile(r'\(define-fun B[0-9]+ \(\) Bool \(= \|(.*)\| \|(.*)\|\)\)')
TAUTOLOGY_FILTER2  = re.compile(r'\(define-fun B[0-9]+ \(\) Bool \(= \((.*)\) \((.*)\)\)\)')
SOURCE_LOCATION_REG= re.compile(r'// ([0-9]+) file (\S+) line ([0-9]+) function (\S+)')

GUARD_PREFIX = 'guard: '
POSTFIXES = ['_A', '_nA']
LINE_FILTERS = [
    'set-info',
    'set-option',
    'set-logic',
    'exit',
    'check-sat',
    'declare-fun array_of',
    'get-value',
    'declare-datatypes ((struct.',
    'define-fun update-struct.',
]

@dataclass
class CBMCVar:
    name: str
    use_idx: Optional[int] = None
    struct_accesses: Tuple[Tuple[str, str], ...] = ()
    
    def identifier(self):
        return (self.name, self.struct_accesses)

    def mangle(self):
        assert self.use_idx
        return f'{self.name}#{self.use_idx}' + ''.join(f'..{"::".join(a)}' for a in self.struct_accesses)

    def mapping_str(self):
        assert self.use_idx
        return f'{self.name}' + ''.join(f'.{a[1]}' for a in self.struct_accesses)

    @staticmethod
    def from_identifier(identifier: Tuple[str, Tuple[Tuple[str, str], ...]], use_idx: int) -> 'CBMCVar':
        return CBMCVar(identifier[0], use_idx, identifier[1])

    @staticmethod
    def from_mangled(varname: str) -> 'CBMCVar':
        """ parses mangled names in the form "varinstance#2..structname1::memberx..structname2::membery" """
        assert varname.count('#') <= 1, varname
        parts = varname.split('..')
        name_parts = parts[0].split('#')
        var = name_parts[0]
        use_idx = None
        if len(name_parts) > 1:
            use_idx = int(name_parts[1])
        struct_accesses = []
        for struct_access in parts[1:]:
            struct_location = struct_access.split("::")
            assert len(struct_location) == 2
            struct_accesses.append(tuple(struct_location))
        return CBMCVar(var, use_idx, tuple(struct_accesses))


def op_to_smt(op: ast.AST) -> str:
    if type(op) == ast.Not:
        return "not"
    if type(op) == ast.And:
        return "and"
    if type(op) == ast.Or:
        return "or"
    assert False, f'{op} unknown'


def py_to_smt(node: ast.AST) -> str:
    if type(node) is ast.Module:
        assert len(node.body) == 1
        return py_to_smt(node.body[0])
    elif type(node) is ast.Expr:
        return py_to_smt(node.value)
    elif type(node) is ast.Name:
        return node.id
    elif type(node) is ast.BoolOp:
        params = ' '.join(py_to_smt(x) for x in node.values)
        return f'({op_to_smt(node.op)} {params})'
    elif type(node) is ast.BinOp:
        params = ' '.join(py_to_smt(x) for x in [node.left, node.right])
        return f'({op_to_smt(node.op)} {params})'
    elif type(node) is ast.UnaryOp:
        return f'({op_to_smt(node.op)} {py_to_smt(node.operand)})'
    else:
        assert False, f'type {type(node)} not handled'


def guard_expression_to_smt(expr: str) -> str:
    expr = expr.replace("\guard#", 'g')
    expr = expr.replace("&&", 'and')
    expr = expr.replace("||", 'or')
    expr = expr.replace("!", 'not ')
    tree = ast.parse(expr)
    smt = py_to_smt(tree)
    return smt


def unmangle_var(varname: str) -> CBMCVar:
    """ converts "mystruct#2..st1::ex..st2::ey")
        to ('mystruct', '2', (('st1', 'ex'), ('st2', 'ey')))
    """
    assert varname.count('#') == 1
    parts = varname.split('..')
    var, use_idx = parts[0].split('#')
    struct_accesses = []
    for struct_access in parts[1:]:
        struct_location = struct_access.split("::")
        assert len(struct_location) == 2
        struct_accesses.append(tuple(struct_location))
    return CBMCVar(var, int(use_idx), tuple(struct_accesses))


def get_value_inject(line: str) -> Optional[str]:
    res = VAL_INJECT_REGEX.match(line)
    if res:
        return res.groups()[0]
    return None


def is_filtered(line: str) -> bool:
    for lf in LINE_FILTERS:
        if line.startswith('('+lf):
            return True
    for sfilter in [BOOL_FILTER1, BOOL_FILTER2, STRUCT_DECL_FILTER]:
        if sfilter.search(line):
            return True
    for tfilter in [TAUTOLOGY_FILTER1, TAUTOLOGY_FILTER2]:
        tautology = tfilter.match(line)
        if tautology:
            p1, p2 = tautology.groups()
            if p1 == p2:
                return True
    return False


def extract_initial_vars_from_graph(equality_edges: dict) -> Dict[CBMCVar, int]:
    initial_vars = {}
    for new_var, old_var in equality_edges.items():
        assert not VAL_INJECTOR_REGEX.match(new_var)
        if VAL_TRANSFER_REGEX.match(new_var):
            continue
        if VAL_INJECTOR_REGEX.match(old_var):
            register_initial_var(initial_vars, new_var, old_var)
        if VAL_TRANSFER_REGEX.match(old_var):
            indirect_var = equality_edges[old_var]
            assert VAL_INJECTOR_REGEX.match(indirect_var)
            register_initial_var(initial_vars, new_var, indirect_var)
    return initial_vars


def register_initial_var(initial_vars, new_var: str, _):
    var = CBMCVar.from_mangled(new_var)
    if not var.use_idx: return
    identifier = var.identifier()
    assert identifier not in initial_vars, f'variable "{var}" already registered'
    initial_vars[identifier] = var.use_idx
        

def register_equality_definition(equality_edges, line):
    """ check if line is a definition in the form '(define-fun |var1| () (...) |var2|) """
    res = DEF_EQUAL_REGEX.match(line)
    if res:
        new_var, old_var = res.groups()
        equality_edges[new_var] = old_var


def register_final_var(final_vars, varname):
    var = CBMCVar.from_mangled(varname)
    if not var.use_idx: return
    identifier = var.identifier()
    prev_use_idx = final_vars.get(identifier, 0)
    final_vars[identifier] = max(prev_use_idx, int(var.use_idx))
    
    
def check_leak(line: str):
    vmatch = LEAK_VAL_REGEX.match(line)
    if vmatch:
        global_var, _param_list, leak_param_idx, cbmc_internal, leak_invocation_idx, var_idx, name_postfix = vmatch.groups()
        assert name_postfix == '', 'postfix not implemented'
        assert int(cbmc_internal) == 0, f'unexpected value {cbmc_internal}'
        assert int(var_idx) == 1, f'unexpected value {var_idx}'
        leak_var_to_global_var[f'leak{leak_invocation_idx}_e{leak_param_idx}'] = global_var
        print('LEAK_VAL_REGEX', len(vmatch.groups()))
    vmatch = LEAK_GUARD_REGEX.match(line)
    if vmatch:
        next_var, guard_var, guarded_var, prev_var = vmatch.groups()
        if guarded_var.startswith('leak_'):
            assert guarded_var not in global_var_to_guard_var
            global_var_to_guard_var[guarded_var] = guard_var



def extract_leak_conditions(path: Path) -> Tuple[dict, dict]:
    log_lines = path.read_text().splitlines()
    leak_conditions: Dict[str, str] = {}
    leak_src_meta: Dict[str, tuple] = {}
    equality_expr = None

    cur_src_position = None
    for l in range(len(log_lines)):
        line = log_lines[l]
        line = line.strip()
        src_match = SOURCE_LOCATION_REG.match(line)
        if src_match:
            cur_src_position = src_match.groups()
        if equality_expr and line.startswith(GUARD_PREFIX):
            guard_expression = line[len(GUARD_PREFIX):]
            if equality_expr[0].startswith('__leak_f') and '_e0' in equality_expr[0]:
                #print('xxx', equality_expr)
                #print(guard_expression)
                #print(guard_expression_to_smt(guard_expression))
                leak_conditions[equality_expr[0]] = guard_expression_to_smt(guard_expression)
                leak_src_meta[equality_expr[0]] = cur_src_position
                #('line '+log_lines[l-2], leak_conditions[equality_expr[0]])
                #leak_src_meta[equality_expr[0]] = ('line '+log_lines[l-2].split(' line ')[1], leak_conditions[equality_expr[0]])
        match = LOG_EQUALITY_REGEX.match(line)
        equality_expr = match.groups() if match else None
    return leak_conditions, leak_src_meta


def decode_cprover_triplet(s: str) -> Tuple[int, int, int]:
    """ '0@2#1' -> (0, 2, 1) """
    thread_id, rest = s.split('@')
    func_invocation, use_idx = rest.split('#')
    return int(thread_id), int(func_invocation), int(use_idx)


def leak_var_to_nums(leakvar):
    var, triplet = leakvar.split('!')
    triplet = decode_cprover_triplet(triplet)
    func, paramIdx = LEAK_NAME_REGEX.match(var).groups()
    return int(func), int(paramIdx), *triplet
    

def create_leak_id_mapping(leak_conditions):
    invocations = {}
    for leakvar in leak_conditions.keys():
        func, paramIdx, num1, invocation, num2 = leak_var_to_nums(leakvar)
        assert num1 == 0, 'unexpected'
        assert num2 == 1, 'unexpected'
        if func in invocations:
            invocations[func].add(invocation)
        else:
            invocations[func] = {invocation}
    leak_mapping = {}
    leak_id = 1
    for func in sorted(invocations.keys()):
        for invocation in invocations[func]:
            #leak_mapping[leak_id] = (func, invocation)
            leak_mapping[(func, invocation)] = leak_id
            leak_id += 1
    return leak_mapping


def create_leak_id_explanation(file, leak_id_mapping, leak_src_meta):
    leak_explanations = {}
    for leakvar, meta in leak_src_meta.items():
        func, paramIdx, num1, invocation, num2 = leak_var_to_nums(leakvar)
        leak_id = leak_id_mapping[(func, invocation)]
        lognum, file, line, function_name = meta
        file = Path(file).name
        leak_explanations[leak_id] = f'leak {leak_id:3d} [{func:2d} parameters]: {file}:{line} {function_name}'
    return leak_explanations


def write_leak_id_explanation(file, leak_explanations) -> None:
    for leak_id in sorted(leak_explanations.keys()):
        file.write(leak_explanations[leak_id])
        file.write('\n')


def main() -> None:
    leak_conditions, leak_src_meta = extract_leak_conditions(Path('build/smtgen-cbmc/contract.log'))
    leak_id_mapping = create_leak_id_mapping(leak_conditions)
    leak_explanations = create_leak_id_explanation(sys.stdout, leak_id_mapping, leak_src_meta)
    write_leak_id_explanation(sys.stdout, leak_explanations)
    with open('leak_mapping.txt', 'w') as f:
        write_leak_id_explanation(f, leak_explanations)

    lines = Path('build/smtgen-cbmc/contract.smt').read_text().splitlines()
    final_vars: Dict[CBMCVar, int] = {}
    equality_edges: Dict[str, str] = {}
    leak_var_to_global_var = {}
    global_var_to_guard_var = {}
    outlines = []
    line_no = 0
    first_postfix = True
    mapping: Dict[str, str] = {'op1': 'op1'}

    for postfix in POSTFIXES:
        for line in lines:
            if len(line) == 0 or line[0] != '(' or is_filtered(line) or '__CPROVER' in line:
                continue
            # replace problematic CBMC output
            line = line.replace('&92;', '/')
            line = line.replace('#constant=', 'constant=')

            if first_postfix:
                register_equality_definition(equality_edges, line)
                check_leak(line)
            line_parts = line.split('|')
            for i in range(1, len(line_parts), 2):
                register_final_var(final_vars, line_parts[i])
                line_parts[i] = line_parts[i] + postfix
            line = '|'.join(line_parts)
            outlines.append(line)
            line_no += 1
        for leak_name, guard_expression in leak_conditions.items():
            func, paramIdx, num1, invocation, num2 = leak_var_to_nums(leak_name)
            leak_id = leak_id_mapping[(func, invocation)]
            name = f'leak{leak_id}_guard{postfix}'
            expr = re.sub(r"g([0-9]+)", fr"|goto_symex::/guard#\g<1>{postfix}|", guard_expression)
            outlines.append(f'(declare-fun |{name}| () Bool)')
            outlines.append(f'(assert (= |{name}| {expr}))')
            mapping[f'leak{leak_id}_is_active_final{postfix}'] = name
        first_postfix = False

    initial_vars: Dict[CBMCVar, int] = extract_initial_vars_from_graph(equality_edges)

    outlines.append('(declare-fun |op1| () (_ BitVec 32))')
    outlines.append('(assert (= |contract_simulate()::1::symbolic_opcode!0@1#2_A| op1))') # TODO generate name from initial_vars?
    outlines.append('(assert (= |contract_simulate()::1::symbolic_opcode!0@1#2_nA| op1))')

    # assign contract result
    for postfix in POSTFIXES:
        num = final_vars[CBMCVar('contract_result').identifier()]
        outlines.append(f'(assert (= |contract_result#{num}{postfix}| (_ bv1 8)))')

    # assign pc, next_pc
    for equal_var in ['pc', 'next_pc']:
        num = initial_vars[CBMCVar(equal_var).identifier()]
        outlines.append(f'(assert (= |{equal_var}#{num}{POSTFIXES[0]}| |{equal_var}#{num}{POSTFIXES[1]}|))')
        num = final_vars[CBMCVar(equal_var).identifier()]
        outlines.append(f'(assert (= |{equal_var}#{num}{POSTFIXES[0]}| |{equal_var}#{num}{POSTFIXES[1]}|))')

    # mapping for contract vars
    for identifier, startnum in initial_vars.items():
        endnum = final_vars[identifier]
        startvar = CBMCVar.from_identifier(identifier, startnum)
        endvar = CBMCVar.from_identifier(identifier, endnum)
        mapvar = startvar.mapping_str()

        if "::" in startvar.name: continue # skip non-global variables

        #if startvar.name == 'pc':
        #    mapvar = 'PC'
        #if startvar.name == 'next_pc':
        #    mapvar = 'nextPC'
        for postfix in POSTFIXES:
            mapping[f'{mapvar}{postfix}'] = f'{startvar.mangle()}{postfix}'
            mapping[f'{mapvar}_final{postfix}'] = f'{endvar.mangle()}{postfix}'
            
        # TODO maybe use final_vars?
        name = f'assert_indep_{mapvar}'
        v0 = mapping[f'{mapvar}_final{POSTFIXES[0]}'] # TODO merge with upper code
        v1 = mapping[f'{mapvar}_final{POSTFIXES[1]}'] # TODO merge with upper code
        outlines.append(f'(declare-fun |{name}| () Bool)')
        outlines.append(f'(assert (or (not |{name}|) (= |{v0}| |{v1}|)))') # (assert_indep implies A = nA)
        mapping[name] = name
        
    # mapping for leaks
    for postfix in POSTFIXES:
        for identifier, endnum in final_vars.items():
            var = CBMCVar.from_identifier(identifier, endnum)
            mangled_name = var.mangle()
            if '__leak_f' not in mangled_name:
                continue
            leak_var = mangled_name.split('::')[1]
            #print(leak_var)
            func, paramIdx, num1, invocation, num2 = leak_var_to_nums(leak_var)
            leak_id = leak_id_mapping[(func, invocation)]
            mapping[f'leak{leak_id}_e{paramIdx}_final{postfix}'] = f'{mangled_name}{postfix}'
            
    # mapping for assert_indep
    for (func, invocation), leak_id in leak_id_mapping.items():
        for paramIdx in range(func):
            name = f'assert_indep_leak{leak_id}_e{paramIdx}'
            v0 = mapping[f'leak{leak_id}_e{paramIdx}_final{POSTFIXES[0]}']
            v1 = mapping[f'leak{leak_id}_e{paramIdx}_final{POSTFIXES[1]}']
            outlines.append(f'(declare-fun |{name}| () Bool)')
            outlines.append(f'(assert (or (not |{name}|) (= |{v0}| |{v1}|)))') # (assert_indep implies A = nA)
            mapping[name] = name
        
    # write mapping
    with open('build/smtgen-cbmc/contract_contract_model.smt2.map', 'w') as f:
        for k in sorted(mapping):
            f.write(f'{k} {mapping[k]}\n')

    # write smt
    Path('build/smtgen-cbmc/contract_contract_model.smt2').write_text('\n'.join(outlines))
    print('model written successfully')

if __name__ == "__main__":
    main()
