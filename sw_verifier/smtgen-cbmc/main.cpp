#include "../contract/contract.h"

// TODO bool are not picked up by smt conversion
reg read_active_1;
reg read_addr_1;
reg read_val_1;

reg write_active_1;
reg write_addr_1;
reg write_val_1;

extern int __keep_var;

bool contract_result;

#include "leaks.cpp"

bv<32> read_mem(bv<32> addr, bv<32> width) {
    read_active_1 = 1;
    read_addr_1 = addr;
    return read_val_1;
}

void write_mem(bv<32> addr, bv<32> width, bv<32> val) {
    write_active_1 = 1;
    //write_active_1 = addr | 1;
    write_addr_1 = addr;
    switch (bv_cast_concrete(width)) {
        case 1: write_val_1 = (write_val_1 & bv_const(0xffffff00, 32)) | (slice(val, 8, 0)); break;
        case 2: write_val_1 = (write_val_1 & bv_const(0xffff0000, 32)) | (slice(val, 16, 0)); break;
        case 4: write_val_1 = val; break;
    }
}

int main() {
    // init runtime registers
    init_register(read_active_1, REG_BITS, 0);
    init_register(read_addr_1, REG_BITS, 0);
    init_register(read_val_1, REG_BITS, 0);
    init_register(write_active_1, REG_BITS, 0);
    init_register(write_addr_1, REG_BITS, 0);
    init_register(write_val_1, REG_BITS, 0);

    read_active_1 = 0;
    write_active_1 = 0;

    contract_init();
    contract_simulate();

    // TODO move to contract

    finalize_register(pc);
    finalize_register(next_pc);

    finalize_register(regs.x1);
    finalize_register(regs.x2);
    finalize_register(regs.x3);
    finalize_register(regs.x4);
    finalize_register(regs.x5);
    finalize_register(regs.x6);
    finalize_register(regs.x7);
    finalize_register(regs.x8);
    finalize_register(regs.x9);
    finalize_register(regs.x10);
    finalize_register(regs.x11);
    finalize_register(regs.x12);
    finalize_register(regs.x13);
    finalize_register(regs.x14);
    finalize_register(regs.x15);
#ifdef RV_I
    finalize_register(regs.x16);
    finalize_register(regs.x17);
    finalize_register(regs.x18);
    finalize_register(regs.x19);
    finalize_register(regs.x20);
    finalize_register(regs.x21);
    finalize_register(regs.x22);
    finalize_register(regs.x23);
    finalize_register(regs.x24);
    finalize_register(regs.x25);
    finalize_register(regs.x26);
    finalize_register(regs.x27);
    finalize_register(regs.x28);
    finalize_register(regs.x29);
    finalize_register(regs.x30);
    finalize_register(regs.x31);
#endif

    finalize_register(prev_regs.x1);
    finalize_register(prev_regs.x2);
    finalize_register(prev_regs.x3);
    finalize_register(prev_regs.x4);
    finalize_register(prev_regs.x5);
    finalize_register(prev_regs.x6);
    finalize_register(prev_regs.x7);
    finalize_register(prev_regs.x8);
    finalize_register(prev_regs.x9);
    finalize_register(prev_regs.x10);
    finalize_register(prev_regs.x11);
    finalize_register(prev_regs.x12);
    finalize_register(prev_regs.x13);
    finalize_register(prev_regs.x14);
    finalize_register(prev_regs.x15);
#ifdef RV_I
    finalize_register(prev_regs.x16);
    finalize_register(prev_regs.x17);
    finalize_register(prev_regs.x18);
    finalize_register(prev_regs.x19);
    finalize_register(prev_regs.x20);
    finalize_register(prev_regs.x21);
    finalize_register(prev_regs.x22);
    finalize_register(prev_regs.x23);
    finalize_register(prev_regs.x24);
    finalize_register(prev_regs.x25);
    finalize_register(prev_regs.x26);
    finalize_register(prev_regs.x27);
    finalize_register(prev_regs.x28);
    finalize_register(prev_regs.x29);
    finalize_register(prev_regs.x30);
    finalize_register(prev_regs.x31);
#endif
    finalize_register(mem_last_addr);
    finalize_register(mem_last_read);

    finalize_register(read_active_1);
    finalize_register(read_addr_1);
    finalize_register(read_val_1);
    finalize_register(write_active_1);
    finalize_register(write_addr_1);
    finalize_register(write_val_1);

    __CPROVER_output("%lx", read_active_1);
    __CPROVER_output("%lx", write_active_1);

    assert(0);
    return 0;
}
