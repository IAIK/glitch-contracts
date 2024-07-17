#include "contract.h"
#include "contract_runtime.h"

reg pc;
reg next_pc;
regfile_t regs;

// shadow / leakagestate regs
regfile_t prev_regs;
reg mem_last_addr;
reg mem_last_read;
bv<5> prev_rd;
bv<5> prev_rs1;
bv<5> prev_rs2;

regfile_t tmp_regs;

bv<32> rX(const regfile_t* regfile, bv<5> r)
{
#ifndef RV_I
    r = r & 0xf; // ignore MSB in RV32E
#endif
    switch (bv_cast_concrete(r))
    {
        case 0:  return 0;
        case 1:  return regfile->x1;
        case 2:  return regfile->x2;
        case 3:  return regfile->x3;
        case 4:  return regfile->x4;
        case 5:  return regfile->x5;
        case 6:  return regfile->x6;
        case 7:  return regfile->x7;
        case 8:  return regfile->x8;
        case 9:  return regfile->x9;
        case 10: return regfile->x10;
        case 11: return regfile->x11;
        case 12: return regfile->x12;
        case 13: return regfile->x13;
        case 14: return regfile->x14;
        case 15: return regfile->x15;
#ifdef RV_I
        case 16: return regfile->x16;
        case 17: return regfile->x17;
        case 18: return regfile->x18;
        case 19: return regfile->x19;
        case 20: return regfile->x20;
        case 21: return regfile->x21;
        case 22: return regfile->x22;
        case 23: return regfile->x23;
        case 24: return regfile->x24;
        case 25: return regfile->x25;
        case 26: return regfile->x26;
        case 27: return regfile->x27;
        case 28: return regfile->x28;
        case 29: return regfile->x29;
        case 30: return regfile->x30;
        case 31: return regfile->x31;
#endif
        default:
            INVALID_OPCODE();
    }
    return 0;
}

void wX(bv<5> r, bv<32> v)
{
    //printf("set register x%s = %s\n", bv_to_dec(r), bv_repr(v));
    switch (bv_cast_concrete(r))
    {
        case 0:          break;
        case 1:  regs.x1 = v; break;
        case 2:  regs.x2 = v; break;
        case 3:  regs.x3 = v; break;
        case 4:  regs.x4 = v; break;
        case 5:  regs.x5 = v; break;
        case 6:  regs.x6 = v; break;
        case 7:  regs.x7 = v; break;
        case 8:  regs.x8 = v; break;
        case 9:  regs.x9 = v; break;
        case 10: regs.x10 = v; break;
        case 11: regs.x11 = v; break;
        case 12: regs.x12 = v; break;
        case 13: regs.x13 = v; break;
        case 14: regs.x14 = v; break;
        case 15: regs.x15 = v; break;
#ifdef RV_I
        case 16: regs.x16 = v; break;
        case 17: regs.x17 = v; break;
        case 18: regs.x18 = v; break;
        case 19: regs.x19 = v; break;
        case 20: regs.x20 = v; break;
        case 21: regs.x21 = v; break;
        case 22: regs.x22 = v; break;
        case 23: regs.x23 = v; break;
        case 24: regs.x24 = v; break;
        case 25: regs.x25 = v; break;
        case 26: regs.x26 = v; break;
        case 27: regs.x27 = v; break;
        case 28: regs.x28 = v; break;
        case 29: regs.x29 = v; break;
        case 30: regs.x30 = v; break;
        case 31: regs.x31 = v; break;
#endif
        default:
            printf("no such register %s\n", bv_repr(r));
            INVALID_OPCODE();
    }
}

regfile_t glitchy_decode(const regfile_t* regfile, bv<5> idx, bv<5> prev_idx) {
    bv<5> mask = (idx ^ prev_idx ^ bv_const(0b11111, 5)) & REGIDX_BITS;
    const bv<5> pattern = (idx & mask);
    regfile_t res = {
        ((bv_const( 1, 5) & mask) == pattern) ? regfile->x1  : 0,
        ((bv_const( 2, 5) & mask) == pattern) ? regfile->x2  : 0,
        ((bv_const( 3, 5) & mask) == pattern) ? regfile->x3  : 0,
        ((bv_const( 4, 5) & mask) == pattern) ? regfile->x4  : 0,
        ((bv_const( 5, 5) & mask) == pattern) ? regfile->x5  : 0,
        ((bv_const( 6, 5) & mask) == pattern) ? regfile->x6  : 0,
        ((bv_const( 7, 5) & mask) == pattern) ? regfile->x7  : 0,
        ((bv_const( 8, 5) & mask) == pattern) ? regfile->x8  : 0,
        ((bv_const( 9, 5) & mask) == pattern) ? regfile->x9  : 0,
        ((bv_const(10, 5) & mask) == pattern) ? regfile->x10 : 0,
        ((bv_const(11, 5) & mask) == pattern) ? regfile->x11 : 0,
        ((bv_const(12, 5) & mask) == pattern) ? regfile->x12 : 0,
        ((bv_const(13, 5) & mask) == pattern) ? regfile->x13 : 0,
        ((bv_const(14, 5) & mask) == pattern) ? regfile->x14 : 0,
        ((bv_const(15, 5) & mask) == pattern) ? regfile->x15 : 0
    };
    return res;
}

#define cond(a, var) ((a)?(var):zero)
#define condrf(a, var) ((a)?(var):zero_regfile)

void regfile_glitches(bv<32> op) {
    const bv<5> rd  = slice(op, 11,  7);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    regfile_t glitchy_rs1_val = glitchy_decode(&regs, rs1, prev_rs1);
    regfile_t glitchy_prev_rs1_val = glitchy_decode(&prev_regs, rs1, prev_rs1);
    regfile_t glitchy_rs2_val = glitchy_decode(&regs, rs2, prev_rs2);
    regfile_t glitchy_prev_rs2_val = glitchy_decode(&prev_regs, rs2, prev_rs2);

    bv<5> mask = (prev_rd ^ rd ^ bv_const(0b11111, 5)) & REGIDX_BITS;
    const bv<5> pattern = (rd & mask);

    const bv<32> zero = bv_const(0, 32);
    regfile_t zero_regfile = {0};

    for (int i = 1; i < NUM_REGS; i++) {
        bool a = ((bv_const( i, 5) & mask) == pattern);
        leak(rX(&regs, i), rX(&prev_regs, i),
             cond(a, mem_last_addr),
             cond(a, mem_last_read),
             rf_to_bv(condrf(a, glitchy_rs1_val)),
             rf_to_bv(condrf(a, glitchy_prev_rs1_val)),
             rf_to_bv(condrf(a, glitchy_rs2_val)),
             rf_to_bv(condrf(a, glitchy_prev_rs2_val))
        );
    }

    leak(mem_last_addr, mem_last_read,
         rf_to_bv(glitchy_rs1_val),
         rf_to_bv(glitchy_prev_rs1_val),
         rf_to_bv(glitchy_rs2_val),
         rf_to_bv(glitchy_prev_rs2_val));
}

void debug_regs()
{
    int r = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (r == 0)
            {
                printf(" PC %s ", bv_repr(pc));
            }
            else
            {
                printf("%sx%d %s ", (r < 10) ? " " : "", r, bv_repr(rX(&regs, r)));
            }
            r++;
        }
        printf("\n");
    }
}

bool execute_rtype(bv<32> op)
{
    const bv<5> rd = slice(op, 11, 7);
    const bv<3> func = slice(op, 14, 12);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    const bv<7> func2 = slice(op, 31, 25);
    const reg rs1_val = rX(&regs, rs1);
    const reg rs2_val = rX(&regs, rs2);
    const bv<5> rs2_val_low = slice(rs2_val, 4, 0);
    reg res;

#ifndef RV_I
    if (slice(rd, 4, 4) != bv_const(0, 1)) goto invalid_rtype;
    if (slice(rs1, 4, 4) != bv_const(0, 1)) goto invalid_rtype;
    if (slice(rs2, 4, 4) != bv_const(0, 1)) goto invalid_rtype;
#endif

    switch (bv_cast_concrete(func2))
    {
    case RV_FUNC2_ARITH0:
        switch (bv_cast_concrete(func))
        {
            case RV_FUNC_ADD : res = rs1_val + rs2_val; break;
            case RV_FUNC_SLT : res = zero_extend(bool_to_bits(signed_lower_than(rs1_val, rs2_val, REG_BITS)), REG_BITS); break;
            case RV_FUNC_SLTU: res = zero_extend(bool_to_bits(rs1_val < rs2_val), REG_BITS); break;
            case RV_FUNC_AND : res = rs1_val & rs2_val; break;
            case RV_FUNC_OR  : res = rs1_val | rs2_val; break;
            case RV_FUNC_XOR : res = rs1_val ^ rs2_val; break;
            case RV_FUNC_SLL : res = rs1_val << rs2_val_low; break;
            case RV_FUNC_SRL : res = rs1_val >> rs2_val_low; break;
            default: goto invalid_rtype;
        }
        break;
    case RV_FUNC2_ARITH1:
        switch (bv_cast_concrete(func))
        {
            case RV_FUNC_SUB: res = rs1_val - rs2_val; break;
            case RV_FUNC_SRA: res = shift_right_arith(rs1_val, rs2_val_low, 32); break;
            default: goto invalid_rtype;
        }
        break;
    default:
        goto invalid_rtype;
    }

    wX(rd, res);
    return true;

invalid_rtype:
    INVALID_OPCODE();
    return false;
}

bool execute_itype(bv<32> op)
{
    const bv<5> rd = slice(op, 11, 7);
    const bv<3> func = slice(op, 14, 12);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    const bv<12> imm = slice(op, 31, 20);
    const bv<5> shamt = slice(op, 24, 20); // only needed for SLLI, SRLI and SRAI
    const bv<7> func2 = slice(op, 31, 25); // only needed for SLLI, SRLI and SRAI
    const reg rs1_val = rX(&regs, rs1);
    const reg rs2_val = rX(&regs, rs2);
    const reg immext = sign_extend(imm, 12, REG_BITS);
    reg res;

#ifndef RV_I
    if (slice(rd, 4, 4) != bv_const(0, 1)) goto invalid_itype;
    if (slice(rs1, 4, 4) != bv_const(0, 1)) goto invalid_itype;
#endif

    //printf("imm    %s\n", bv_to_hex(imm));
    //printf("immext %s\n", bv_to_hex(immext));

    switch (bv_cast_concrete(func))
    {
        case RV_FUNC_ADDI : res = rs1_val + immext; break;
        case RV_FUNC_SLTI : res = zero_extend(bool_to_bits(signed_lower_than(rs1_val, immext, REG_BITS)), REG_BITS); break;
        case RV_FUNC_SLTIU: res = zero_extend(bool_to_bits(rs1_val < immext), REG_BITS); break;
        case RV_FUNC_XORI : res = rs1_val ^ immext; break;
        case RV_FUNC_ORI  : res = rs1_val | immext; break;
        case RV_FUNC_ANDI : res = rs1_val & immext; break;
        case RV_FUNC_SLLI :
            switch (bv_cast_concrete(func2))
            {
                case RV_FUNC2_SLLI: res = rs1_val << shamt; break;
                default: goto invalid_itype;
            }
            break;
        case RV_FUNC_SHIFT_RIGHT:
            switch (bv_cast_concrete(func2))
            {
                case RV_FUNC2_SRLI: res = rs1_val >> shamt; break;
                case RV_FUNC2_SRAI: res = shift_right_arith(rs1_val, shamt, 32); break;
                default: goto invalid_itype;
            }
            break;
        default: goto invalid_itype;
    }

    wX(rd, res);
    return true;

invalid_itype:
    INVALID_OPCODE();
    return false;
}

bool execute_btype(bv<32> op)
{
    const bv<1>  imm11_11 = slice(op,  7,  7);
    const bv<4>  imm04_01 = slice(op, 11,  8);
    const bv<3>  func     = slice(op, 14, 12);
    const bv<5>  rs1      = slice(op, 19, 15);
    const bv<5>  rs2      = slice(op, 24, 20);
    const bv<6>  imm10_05 = slice(op, 30, 25);
    const bv<1>  imm12_12 = slice(op, 31, 31);
    const bv<13> imm = (zero_extend(imm12_12, 13) << 12)
                     | (zero_extend(imm11_11, 13) << 11)
                     | (zero_extend(imm10_05, 13) <<  5)
                     | (zero_extend(imm04_01, 13) <<  1);

    const reg rs1_val = rX(&regs, rs1);
    const reg rs2_val = rX(&regs, rs2);
    const reg immext = sign_extend(imm, 13, REG_BITS);
    const reg target = pc + immext;
    bool taken;

#ifndef RV_I
    if (slice(rs1, 4, 4) != bv_const(0, 1)) goto invalid_btype;
    if (slice(rs2, 4, 4) != bv_const(0, 1)) goto invalid_btype;
#endif

    //printf("imm    %s\n", bv_to_hex(imm));
    //printf("immext %s\n", bv_to_hex(immext));

    switch (bv_cast_concrete(func))
    {
        case RV_FUNC_BEQ : taken = rs1_val == rs2_val; break;
        case RV_FUNC_BNE : taken = rs1_val != rs2_val; break;
        case RV_FUNC_BLT : taken = signed_lower_than(rs1_val, rs2_val, REG_BITS); break;
        case RV_FUNC_BGE : taken = signed_greater_equal(rs1_val, rs2_val, REG_BITS); break;
        case RV_FUNC_BLTU: taken = rs1_val < rs2_val; break;
        case RV_FUNC_BGEU: taken = rs1_val >= rs2_val; break;
        default: goto invalid_btype;
    }
    if (taken)
    {
        next_pc = target;
    }
    
    return true;

invalid_btype:
    INVALID_OPCODE();
    return false;
}

bool execute_load(bv<32> op)
{
    const bv<5> rd = slice(op, 11, 7);
    const bv<2> width = slice(op, 13, 12);
    const bv<1> is_unsigned = slice(op, 14, 14);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    const bv<12> imm = slice(op, 31, 20);
    const reg rs1_val = rX(&regs, rs1);
    const reg offset = sign_extend(imm, 12, REG_BITS);
    const reg addr = rs1_val + offset;
    reg res;

#ifndef RV_I
    if (slice(rd, 4, 4) != bv_const(0, 1)) goto invalid_load;
    if (slice(rs1, 4, 4) != bv_const(0, 1)) goto invalid_load;
#endif

    const reg req_data = isa_read_mem(op, addr);

    switch (bv_cast_concrete(width))
    {
        case RV_WIDTH_BYTE  : {
                const bv<8>  val = crop_mem_8 (addr, req_data);
                res = ((is_unsigned == 1) ? zero_extend(val, REG_BITS) : sign_extend(val,  8, REG_BITS));
            } break;
        case RV_WIDTH_HALF  : {
                if ((addr & (2-1)) != 0) goto invalid_load;
                const bv<16> val = crop_mem_16(addr, req_data);
                res = ((is_unsigned == 1) ? zero_extend(val, REG_BITS) : sign_extend(val, 16, REG_BITS));
            } break;
        case RV_WIDTH_WORD  : {
                if ((addr & (4-1)) != 0) goto invalid_load;
                const bv<32> val = crop_mem_32(addr, req_data);
                res = ((is_unsigned == 1) ? zero_extend(val, REG_BITS) : sign_extend(val, 32, REG_BITS));
            } break;
#ifdef RV_64
        case RV_WIDTH_DOUBLE: {const bv<64> val = isa_read_mem_64(rs1_val, rs2_val, addr); res = ((is_unsigned == 1) ? zero_extend(val, REG_BITS) : sign_extend(val, 64, REG_BITS));} break;
#endif
        default: goto invalid_load;
    }

    wX(rd, res);
    return true;

invalid_load:
    INVALID_OPCODE();
    return false;
}

bool execute_store(bv<32> op)
{
    const bv<5> imm1 = slice(op, 11, 7);
    const bv<3> func = slice(op, 14, 12);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    const bv<7> imm2 = slice(op, 31, 25);
    const bv<12> imm = (zero_extend(imm2, 12) << 5) | zero_extend(imm1, 12); 
    const bv<3> width = func;
    const reg rs1_val = rX(&regs, rs1);
    const reg rs2_val = rX(&regs, rs2);
    const reg offset = sign_extend(imm, 12, REG_BITS);
    const reg addr = rs1_val + offset;

#ifndef RV_I
    if (slice(rs1, 4, 4) != bv_const(0, 1)) goto invalid_store;
    if (slice(rs2, 4, 4) != bv_const(0, 1)) goto invalid_store;
#endif

    mem_last_addr = addr;

    switch (bv_cast_concrete(width))
    {
        case RV_WIDTH_BYTE  : write_mem(addr, 1, slice(rs2_val,  7, 0)); break;
        case RV_WIDTH_HALF  : write_mem(addr, 2, slice(rs2_val, 15, 0)); break;
        case RV_WIDTH_WORD  : write_mem(addr, 4, slice(rs2_val, 31, 0)); break;
#ifdef RV_64
        case RV_WIDTH_DOUBLE: write_mem(addr, 8, slice(rs2_val, 63, 0)); break;
#endif
        default: goto invalid_store;
    }

    return true;

invalid_store:
    INVALID_OPCODE();
    return false;
}

bool execute_utype(bv<32> op)
{
    const bv<7> opcode = slice(op, 6, 0);
    const bv<5> rd = slice(op, 11, 7);
    const bv<20> imm20 = slice(op, 31, 12);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    const bv<32> offset = (zero_extend(imm20, 32) << 12);
    const reg rs1_val = rX(&regs, rs1);
    const reg rs2_val = rX(&regs, rs2);
    reg res; 

#ifndef RV_I
    if (slice(rd, 4, 4) != bv_const(0, 1)) return error("invalid UTYPE");
#endif

    //printf("rd: %s\n",  bv_repr(rd));
    //printf("imm20: %s\n", bv_repr(imm20));
    //printf("imm20ext: %s\n", bv_repr(zero_extend(imm20, 32)));
    //printf("offset: %s\n", bv_repr(offset));

    switch (bv_cast_concrete(opcode))
    {
        case RV_OPC_LUI : res = offset; break;
        case RV_OPC_AUIP: res = pc + offset; break;
        default: return error("invalid UTYPE");
    }

    wX(rd, res);
    return true;

}


bool execute_jal(bv<32> op)
{
    const bv<5>  rd = slice(op, 11, 7);
    const bv<8>  imm19_12 = slice(op, 19, 12);
    const bv<1>  imm11_11 = slice(op, 20, 20);
    const bv<10> imm10_01 = slice(op, 30, 21);
    const bv<1>  imm20_20 = slice(op, 31, 31);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    const bv<21> imm = (zero_extend(imm20_20, 21) << 20)
                     | (zero_extend(imm19_12, 21) << 12)
                     | (zero_extend(imm11_11, 21) << 11)
                     | (zero_extend(imm10_01, 21) <<  1);
    const bv<32> addr = pc + sign_extend(imm, 21, REG_BITS);
    const reg rs1_val = rX(&regs, rs1);
    const reg rs2_val = rX(&regs, rs2);

    //printf("rd: %x\n", rd);
    //printf("imm: %x\n", imm);
    //printf("addr: %x\n", addr);
    
    wX(rd, next_pc);
    next_pc = addr;
    return true;
}

bool execute_jalr(bv<32> op)
{
    const bv<5> rd = slice(op, 11, 7);
    const bv<3> func = slice(op, 14, 12);
    const bv<5> rs1 = slice(op, 19, 15);
    const bv<5> rs2 = slice(op, 24, 20);
    const bv<12> imm = slice(op, 31, 20);
    const reg rs1_val = rX(&regs, rs1);
    const reg rs2_val = rX(&regs, rs2);
    const reg offset = sign_extend(imm, 12, REG_BITS);
    const reg addr = (rs1_val + offset) & ~1;

    if (func != bv_const(0b000, 3))
        return error("invalid JALR");

    wX(rd, next_pc);
    next_pc = addr;
    return true;
}

void save_opcode_info(const bv<32> op)
{
    prev_rd  = slice(op, 11,  7);
    prev_rs1 = slice(op, 19, 15);
    prev_rs2 = slice(op, 24, 20);
}

bool execute(const bv<32> op)
{
    tmp_regs = regs;
    bool ret = false;
    const bv<7> opcode = slice(op, 6, 0);

    if ((pc & 0x3) != 0) return error("pc unaligned");

    if(bv_cast_concrete(opcode) != RV_OPC_LOAD) 
    {
        regfile_glitches(op);
    }
    regfile_glitches(op);

    //printf("opcode %s: ", bv_to_hex(opcode));
    switch (bv_cast_concrete(opcode))
    {
        case RV_OPC_RTYPE : printf("rtype\n"); ret = execute_rtype(op); break;
        case RV_OPC_ITYPE : printf("itype\n"); ret = execute_itype(op); break;
        case RV_OPC_BTYPE : printf("btype\n"); ret = execute_btype(op); break;
        case RV_OPC_LOAD  : printf("load \n"); ret = execute_load (op); break;
        case RV_OPC_STORE : printf("store\n"); ret = execute_store(op); break;
        case RV_OPC_LUI   : // fallthrough
        case RV_OPC_AUIP  : printf("utype\n"); ret = execute_utype(op); break;
        case RV_OPC_JAL   : printf("jal  \n"); ret = execute_jal  (op); break;
        case RV_OPC_JALR  : printf("jalr \n"); ret = execute_jalr (op); break;
        case RV_OPC_FENCE : // FENCE, FENCE.I
        case RV_OPC_CSRENV: // ECALL, EBREAK, CSR ops
            return error("opcode not allowed");
        default: 
            return error("opcode invalid");
    }
    save_opcode_info(op);
    prev_regs = tmp_regs;
    if(bv_cast_concrete(opcode) != RV_OPC_LOAD)
    {
        mem_last_read = 0;
    }

    return ret;
}

bool step_cpu(bv<32> op)
{
    printf("----------\n");
    //debug_mem();
    debug_regs();
    printf("executing %s @ %s\n", bv_to_hex(op), bv_to_hex(pc));
    next_pc = pc + 4;
    const bool ret = execute(op);
    pc = next_pc;
    return ret;
}

extern reg get_symbolic();

void regfile_init(regfile_t* regfile) {
    init_register(regfile->x1, REG_BITS, 0);
    init_register(regfile->x2, REG_BITS, 0);
    init_register(regfile->x3, REG_BITS, 0);
    init_register(regfile->x4, REG_BITS, 0);
    init_register(regfile->x5, REG_BITS, 0);
    init_register(regfile->x6, REG_BITS, 0);
    init_register(regfile->x7, REG_BITS, 0);
    init_register(regfile->x8, REG_BITS, 0);
    init_register(regfile->x9, REG_BITS, 0);
    init_register(regfile->x10, REG_BITS, 0);
    init_register(regfile->x11, REG_BITS, 0);
    init_register(regfile->x12, REG_BITS, 0);
    init_register(regfile->x13, REG_BITS, 0);
    init_register(regfile->x14, REG_BITS, 0);
    init_register(regfile->x15, REG_BITS, 0);
#ifdef RV_I
    init_register(regfile->x16, REG_BITS, 0);
    init_register(regfile->x17, REG_BITS, 0);
    init_register(regfile->x18, REG_BITS, 0);
    init_register(regfile->x19, REG_BITS, 0);
    init_register(regfile->x20, REG_BITS, 0);
    init_register(regfile->x21, REG_BITS, 0);
    init_register(regfile->x22, REG_BITS, 0);
    init_register(regfile->x23, REG_BITS, 0);
    init_register(regfile->x24, REG_BITS, 0);
    init_register(regfile->x25, REG_BITS, 0);
    init_register(regfile->x26, REG_BITS, 0);
    init_register(regfile->x27, REG_BITS, 0);
    init_register(regfile->x28, REG_BITS, 0);
    init_register(regfile->x29, REG_BITS, 0);
    init_register(regfile->x30, REG_BITS, 0);
    init_register(regfile->x31, REG_BITS, 0);
#endif
}

void contract_init()
{
    init_register(pc, REG_BITS, 0x80);
    init_register(next_pc, REG_BITS, 0);
    regfile_init(&regs);

    regfile_init(&prev_regs);
    init_register(mem_last_addr, REG_BITS, 0);
    init_register(mem_last_read, REG_BITS, 0);
    init_register(prev_rd, 5, 0);
    init_register(prev_rs1, 5, 0);
    init_register(prev_rs2, 5, 0);
}

#ifndef CONTRACT_MODE_SIMULATOR
extern bool contract_result;
#endif

void contract_simulate()
{
#ifdef CONTRACT_MODE_SIMULATOR
    while (step_cpu(read_mem(pc, 4)));
#else
    reg symbolic_opcode = get_symbolic();
    contract_result = step_cpu(symbolic_opcode);
#endif
}

#ifdef CONTRACT_MODE_SIMULATOR
int set_register_word(const char *name, bv<32> value)
{
    printf("setting register %s = %s\n", name, bv_repr(value));
    if (strcmp(name, "pc") == 0)
    {
        pc = value;
    }
    else if (name[0] == 'x')
    {
        const int r = atoi(name + 1);
        wX(r, value);
    }
    else
    {
        printf("error setting register\n");
        exit(0);
    }
    return 0;
}
#endif
