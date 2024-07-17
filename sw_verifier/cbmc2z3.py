#from pysmt.smtlib.parser import SmtLibParser
from pathlib import Path
import sys
from typing import Any, List

# https://gist.github.com/roberthoenig/30f08b64b6ba6186a2cdee19502040b4
# Parse input string into a list of all parentheses and atoms (int or str),
# exclude whitespaces.
def normalize_str(string: str) -> List[str]:
    str_norm = []
    last_c = ')'
    escape = False
    for c in string:
        if c == '|':
            if escape:
                str_norm[-1] += c
            else:
                str_norm.append(c)
            escape = not escape
            continue
        if escape:
            str_norm[-1] += c
            continue
        if c.isalnum() or (c not in '() ' and c != None):
            #str_norm[-1] += c
            if last_c.isalnum() or last_c not in '() ':
                str_norm[-1] += c
            else:
                str_norm.append(c)
        elif not c.isspace():
            str_norm.append(c)
        last_c = c
    return str_norm

# Generate abstract syntax tree from normalized input.
def get_ast(input_norm: List[str]) -> List[Any]:
    ast = []
    # Go through each element in the input:
    # - if it is an open parenthesis, find matching parenthesis and make recursive
    #   call for content in-between. Add the result as an element to the current list.
    # - if it is an atom, just add it to the current list.
    i = 0
    while i < len(input_norm):
        symbol = input_norm[i]
        if symbol == '(':
            list_content = []
            match_ctr = 1 # If 0, parenthesis has been matched.
            while match_ctr != 0:
                i += 1
                if i >= len(input_norm):
                    raise ValueError("Invalid input: Unmatched open parenthesis.")
                symbol = input_norm[i]
                if symbol == '(':
                    match_ctr += 1
                elif symbol == ')':
                    match_ctr -= 1
                if match_ctr != 0:
                    list_content.append(symbol)             
            ast.append(get_ast(list_content))
        elif symbol == ')':
                raise ValueError("Invalid input: Unmatched close parenthesis.")
        else:
            try:
                ast.append(int(symbol))
            except ValueError:
                ast.append(symbol)
        i += 1
    return ast

# (define-fun |leak_e1#108| () (_ BitVec 32) (ite |goto_symex::&92;guard#235| |leak_e1#107| |leak_e1#1|))
# (define-fun |leak_e2#6| () (_ BitVec 32) |
# leak(unsigned_int,unsigned_int)::e2!0@3#1

# leak(unsigned_int,unsigned_int,unsigned_int,unsigned_int)::e1!0@1#1
# -> leak(unsigned_int,unsigned_int,unsigned_int,unsigned_int)::$tmp::return_value_leak_hash!0@1#1

postfixes = ['_A', '_nA']
#postfixes = ['_A']

line_filters = [
    'set-info',
    'set-option',
    'set-logic',
    'exit',
    'check-sat',
    'declare-fun array_of',
    #'define-fun |x7#1|',
    #'define-fun |x8#1|',
    #'define-fun |x23#1|',
]

final_vars = {}
initial_vars = {}
leak_var_to_global_var = {}
global_var_to_guard_var = {}

def get_value_inject(line):
    match = VAL_INJECT_REGEX.match(line)
    if match:
        return match.groups()[0]
    return None

def is_filtered(line):
    for lf in line_filters:
        if line.startswith('('+lf):
            return True
    return False

def register_initial_var(initial_vars, line):
     first_var = get_value_inject(line)
     if first_var:
        #print(line)
        var, num = first_var.split('#')
        #if var in final_vars:
            #print(var, num, initial_vars)
        assert var not in initial_vars
        #prev = final_vars.get(var, 0)
        #next = max(prev, int(num))
        initial_vars[var] = num

def register_final_var(final_vars, varname):
    if varname.count('#') != 1:
        #print(varname)
        return
    var, num = varname.split('#')
    prev = final_vars.get(var, 0)
    next = max(prev, int(num))
    final_vars[var] = next
    
def check_leak(line):
    match = LEAK_VAL_REGEX.match(line)
    if match:
        global_var, _param_list, leak_param_idx, cbmc_internal, leak_invocation_idx, var_idx = match.groups()
        assert int(cbmc_internal) == 0, f'unexpected value {cbmc_internal}'
        assert int(var_idx) == 1, f'unexpected value {var_idx}'
        leak_var_to_global_var[f'leak{leak_invocation_idx}_e{leak_param_idx}'] = global_var
        print('LEAK_VAL_REGEX', len(match.groups()))
    #if 'ite |goto_symex' in line:
    #    print(line)
    match = LEAK_GUARD_REGEX.match(line)
    if match:
        next_var, guard_var, guarded_var, prev_var = match.groups()
        if guarded_var.startswith('leak_'):
            assert guarded_var not in global_var_to_guard_var
            global_var_to_guard_var[guarded_var] = guard_var
    return None

# convert
# ( define-fun <name-symbol> ((<symbol> <sort-expr>)*) <sort-expr> <expr>)
# into
# ( declare-fun <name-symbol> (<sort-expr>*) <sort-expr>)
# ( assert (= (<symbol> (<symbol>)*) <expr>) )

def to_str(clause):
    if type(clause) == str:
        return clause
    if type(clause) == int:
        return str(clause)
    if type(clause) == list:
        return '(' + ' '.join(to_str(x) for x in clause) + ')'

def process_lines(lines):
    outlines = []
    for line in lines:
        ast = get_ast(normalize_str(line))
        if len(ast) == 0 or len(ast[0]) == 0 or ast[0][0] != 'define-fun':
            outlines.append(line)
            continue
        #print(ast[0])
        d = ast[0]
        name_symbol = d[1]
        param_names = [x[0] for x in d[2]]
        param_sorts = [x[1] for x in d[2]]
        sort = d[3]
        expr = d[4]
        function_call = name_symbol if len(param_names) == 0 else [name_symbol, param_names]
        out1 = ['declare-fun', name_symbol, param_sorts, sort]
        out2 = ['assert', ['=', function_call, expr]]
        outlines.append(to_str(out1))
        outlines.append(to_str(out2))
        #assert len(param_sorts) == 0
        #outlines.append(to_str(['define-const', name_symbol, sort, expr]))
    return outlines

def process(infile, outfile):
    lines = Path(infile).read_text().splitlines()
    outlines = process_lines(lines)
    Path(outfile).write_text('\n'.join(outlines))

if __name__ == "__main__":
    process(sys.argv[1], sys.argv[2])
