#pragma once
#include "contract_runtime.h"
#include "rv_constants.h"

#define INSTRUCTION_POINTER (pc)

#define signed_greater_equal(a, b, size) (!signed_lower_than(a, b, size))

#ifdef RV_64
#define REG_BITS 64
#else
#define REG_BITS 32
#endif

#ifdef RV_I
#define NUM_REGS 32
#define REGIDX_BITS bv_const(0b11111, 5)
#else
#define NUM_REGS 2
#define REGIDX_BITS bv_const(0b01111, 5)
#endif

#define reg bv<REG_BITS>

#define rf_to_bv(r) r.x1, r.x2, r.x3, r.x4, r.x5, r.x6, r.x7, r.x8, r.x9, r.x10, r.x11, r.x12, r.x13, r.x14, r.x15

typedef struct regfile {
    reg x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
#ifdef RV_I
    reg x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31;
#endif
} regfile_t;

extern reg pc;
extern reg next_pc;
extern regfile_t regs;
extern regfile_t prev_regs;
extern regfile_t tmp_regs;

extern reg mem_last_addr;
extern reg mem_last_read;
extern bv<5> prev_rd;
extern bv<5> prev_rs1;
extern bv<5> prev_rs2;

bv<32> isa_read_mem(const reg op, const reg addr);

bv<8> crop_mem_8(const reg addr, const reg req_data);
bv<16> crop_mem_16(const reg addr, const reg req_data);
bv<32> crop_mem_32(const reg addr, const reg req_data);

regfile_t glitchy_decode(const regfile_t* regfile, bv<5> idx, bv<5> prev_idx);
bv<32> rX(const regfile_t* regfile, bv<5> r);

void contract_init();
int set_register_word(const char* name, bv<32> value);
void contract_simulate();
bool step_cpu(bv<32> op);
