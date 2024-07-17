#!/usr/bin/env python3

N = 32 # max bv size

def o(s):
    print(s.rstrip(' ').strip('\n'))

o(r"""
#include <fmt/core.h>
#include <iostream>
#include <string>
#include <string_view>
#include "contract_runtime.h"
#include "SymbolicManager.h"
#include "bitvector.h"

""")

o(r"""
void* bv_fragment_resize(void* fragment, long source_size, long target_size, long offset)
{
    // printf("bv_fragment_resize (%ld -> %ld)\n", source_size, target_size);
    switch (source_size) {
""")
for source_size in range(1, N+1):
    o(rf"""
        case {source_size}: switch (target_size) {{
    """)
#    for target_size in range(1, source_size):
#        o(rf"""
#            case {target_size}: return new bv_frag<{target_size}>((*(bv_frag<{source_size}>*)(fragment)).slice_impl<{target_size}>(offset));
#        """)
#    o(rf"""
#            case {source_size}: return new bv_frag<{source_size}>((*(bv_frag<{source_size}>*)(fragment)));
#    """)
    for target_size in range(1, N+1):
        if target_size < source_size:
            o(rf"""
            case {target_size}: return new bv_frag<{target_size}>((*(bv_frag<{source_size}>*)(fragment)).slice_impl<{target_size}>(offset));
            """)
        if target_size == source_size:
            o(rf"""
            case {source_size}: return new bv_frag<{source_size}>((*(bv_frag<{source_size}>*)(fragment)));
            """)
        if target_size > source_size:
            o(rf"""
            case {target_size}: return new bv_frag<{target_size}>((*(bv_frag<{source_size}>*)(fragment)).zero_extend_impl<{target_size}>());
            """)
    o(r"""
        }
    """)
o(r"""
    }
    printf("bv_fragment_resize failed (%ld -> %ld not implemented)\n", source_size, target_size);
    assert(false);
}

""")

o(r"""
void* bv_fragment_resize_signed(void* fragment, long source_size, long target_size, long offset)
{
    // printf("bv_fragment_resize_signed (%ld -> %ld)\n", source_size, target_size);
    // TODO check if safe
    // if (source_size == target_size)
    //     return fragment;
    switch (source_size) {
""")
for source_size in range(2, N+1):
    o(rf"""
        case {source_size}: switch (target_size) {{
    """)
#    for target_size in range(1, source_size):
#        o(rf"""
#            case {target_size}: return new bv_frag<{target_size}>((*(bv_frag<{source_size}>*)(fragment)).slice_impl<{target_size}>(offset));
#        """)
#    o(rf"""
#            case {source_size}: return new bv_frag<{source_size}>((*(bv_frag<{source_size}>*)(fragment)));
#    """)
    for target_size in range(2, N+1):
        if target_size < source_size:
            o(rf"""
            case {target_size}: return new bv_frag<{target_size}>((*(bv_frag<{source_size}>*)(fragment)).slice_impl<{target_size}>(offset));
            """)
        if target_size == source_size:
            o(rf"""
            case {source_size}: return new bv_frag<{source_size}>((*(bv_frag<{source_size}>*)(fragment)));
            """)
        if target_size > source_size:
            o(rf"""
            case {target_size}: return new bv_frag<{target_size}>((*(bv_frag<{source_size}>*)(fragment)).sign_extend_impl<{target_size}>());
            """)
    o(r"""
        }
    """)
o(r"""
    }
    printf("bv_fragment_resize_signed failed (%ld -> %ld not implemented)\n", source_size, target_size);
    assert(false);
}

""")

o(r"""
void* bv_fragment_op(bv_op op, long size, void* a, void* b)
{
    switch (size) {
""")
for size in range(1, N+1):
    o(rf"""
        case {size}: switch (op) {{
            case bv_op::AND: return new bv_frag<{size}>(*((bv_frag<{size}>*)a) & *((bv_frag<{size}>*)b));
            case bv_op::OR : return new bv_frag<{size}>(*((bv_frag<{size}>*)a) | *((bv_frag<{size}>*)b));
            case bv_op::XOR: return new bv_frag<{size}>(*((bv_frag<{size}>*)a) ^ *((bv_frag<{size}>*)b));
        }}
    """)
o(r"""
    }
    printf("bv_fragment_op failed (op=%d size=%ld)\n", op, size);
    assert(false);
}
""")


o(r"""
bool bv_fragment_cmp(bv_cmp_op op, long size, void* a, void* b)
{
    switch (size) {
""")
for size in range(1, N+1):
    o(rf"""
        case {size}: switch (op) {{
            case bv_cmp_op::EQ : return *((bv_frag<{size}>*)a) == *((bv_frag<{size}>*)b);
            case bv_cmp_op::LT : return *((bv_frag<{size}>*)a) < *((bv_frag<{size}>*)b);
    """)
    if size >= 2:
        o(rf"""
            case bv_cmp_op::SLT: return (*((bv_frag<{size}>*)a)).signed_lower_than_impl(*((bv_frag<{size}>*)b));
        """)
    o(rf"""
        }}
    """)
o(r"""
    }
    printf("bv_fragment_cmp failed (op=%d size=%ld)\n", op, size);
    assert(false);
}

""")
o(r"""
void bv_fragment_del(long size, void* a)
{
    switch (size) {
""")
for size in range(1, N+1):
    o(rf"""
        case {size}: delete ((bv_frag<{size}>*)a); return;
    """)
o(r"""
    }
    printf("bv_fragment_del(%ld, *) failed. not implemented\n", size);
    assert(false);
}
""")

o(r"""
bool bv_fragment_symbolic(long size, void* a)
{
    switch (size) {
""")
for size in range(1, N+1):
    o(rf"""
        case {size}: return ((bv_frag<{size}>*)a)->get_is_symbolic();
    """)
o(r"""
    }
    printf("bv_fragment_symbolic(%ld, *) failed. not implemented\n", size);
    assert(false);
}
""")

o(r"""
void* bv_fragment_new(long size, long val)
{
    switch (size) {
""")
for size in range(1, N+1):
    o(rf"""
        case {size}: return new bv_frag<{size}>(val);
    """)
o(r"""
    }
    printf("bv_fragment_new(%ld, *) failed. not implemented\n", size);
    assert(false);
}
""")

o(r"""
void* bv_fragment_new(long size, std::shared_ptr<SymbolicExpression> val)
{
    switch (size) {
""")
for size in range(1, N+1):
    o(rf"""
        case {size}: return new bv_frag<{size}>(val);
    """)
o(r"""
    }
    printf("bv_fragment_new(%ld, *) failed. not implemented\n", size);
    assert(false);
}
""")


