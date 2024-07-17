#pragma once
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "../common/contract_runtime_common.h"

#define bv __CPROVER::unsignedbv
#define sbv __CPROVER::signedbv

extern bool contract_result;

#define GET_INITIAL_VAL_HELPER(size) initial_val_helper_##size
bv<32> initial_val_helper_32();
bv<5> initial_val_helper_5();
void final_val_helper(bv<32>);

#define bool_to_bits(b) (b)
#define zero_extend(b, size) (b)
#define slice(b, end, start) ((b >> start) & ((1ul << (end-start+1)) - 1))

#define printf(format, ...) __CPROVER_printf(format, __VA_ARGS__)
#define fprintf(format, ...) do {} while(0)
#define printf(format, ...) do {} while(0)
#define INVALID_OPCODE() do {} while(0)
#define error(msg) (false)
#define bv_cast_concrete(b) (b)
//#define init_register(regname, size, initial_val) {reg regname ## _initial; regname = regname ## _initial;}
#define init_register(regname, size, initial_val) {regname = GET_INITIAL_VAL_HELPER(size)();}
#define finalize_register(regname) {final_val_helper(regname); __CPROVER_output("%lx", regname);}
#define sign_extend(b, old_size, new_size) ((sbv<new_size>)(sbv<old_size>)(b))
#define signed_lower_than(a, b, size) ((sbv<size>)(a) < (sbv<size>)(b))
#define shift_right_arith(b, amm, size) ((sbv<size>)(b) >> (amm))
#define debug_mem()
#define bv_const(value, size) zero_extend((value), (size))

// TODO
//#define read_mem(addr, width) (0)
//#define write_mem(addr, width, val) (0)
bv<32> read_mem(bv<32> addr, bv<32> width);
void write_mem(bv<32> addr, bv<32> width, bv<32> val);

#include "leaks.h"

//void leak(bv<32>);
//void leak(bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>);
//
//void leak(bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>, bv<32>,
//          bv<32>, bv<32>);
