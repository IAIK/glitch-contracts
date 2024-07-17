#include "contract.h"
#include "contract_runtime.h"

#define cond32(a, var) ((a)?(var):bv_const(0, 32))

void load_leakage(const reg op, reg addr, reg req_data)
{
    const bv<5> rd  = slice(op, 11,  7);
    const bv<5> rs1 = slice(op, 19, 15);

    for (int i = 1; i < NUM_REGS; i++) {
        bool a = (bv_const( i, 5) == rd);
        leak(rX(&regs, i),
             cond32(a, rX(&regs, rs1)),
             cond32(a, mem_last_addr),
             cond32(a, req_data)
        );
    }

    leak(rX(&regs, rs1),
         mem_last_addr,
         req_data
    );

    mem_last_read = req_data;
    mem_last_addr = addr;
}

bv<32> isa_read_mem(const reg op, const reg addr)
{
    const reg bus_addr = addr & bv_const(0xfffffffc, 32);
    const reg req_data = read_mem(bus_addr, 4);
    load_leakage(op, addr, req_data);
    return req_data;
}

bv<8> crop_mem_8(const reg addr, const reg req_data)
{
    switch (bv_cast_concrete(slice(addr, 1, 0))) {
        case 0b00: return slice(req_data,  7,  0);
        case 0b01: return slice(req_data, 15,  8);
        case 0b10: return slice(req_data, 23, 16);
        case 0b11: return slice(req_data, 31, 24);
    };
    return 0;
}

bv<16> crop_mem_16(const reg addr, const reg req_data)
{
    switch (bv_cast_concrete(slice(addr, 1, 1))) {
        case 0b0: return slice(req_data, 15,  0);
        case 0b1: return slice(req_data, 31, 16);
    };
    return 0;
}

bv<32> crop_mem_32(const reg addr, const reg req_data)
{
    return req_data;
}
