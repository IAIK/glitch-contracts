from typing import TextIO

def write_def(f: TextIO, l: int):
    f.write('void leak(' + ', '.join(['bv<32>']*l) + ');\n')
    
def write_helper(f: TextIO, l: int):
    f.write(f'extern int leak_e{l}f(bv<32>);\n')

def write_impl(f: TextIO, l: int):
    f.write('void leak(')
    for i in range(l):
        f.write(f'bv<32> __leak_f{l}_e{i}, ')
        if i % 5 == 4:
            f.write('\n          ')
    f.write(') {\n')
    for i in range(l):
        f.write(f'    __keep_var += leak_e{i}f(__leak_f{l}_e{i});\n')
    f.write('}\n')
    
SIZES = list(range(1, 100))

with open('leaks.h', 'w') as f:
    f.write('// AUTO GENERATED FILE, DO NOT MODIFY\n')
    for size in SIZES:
        write_def(f, size)

with open('leaks.cpp', 'w') as f:
    f.write('// AUTO GENERATED FILE, DO NOT MODIFY\n')
    for size in range(0, max(SIZES)+1):
        write_helper(f, size)
    f.write('\n')
    for size in SIZES:
        write_impl(f, size)
