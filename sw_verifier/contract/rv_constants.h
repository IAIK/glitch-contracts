#pragma once

#define RV_OPC_RTYPE    (0b0110011u) //  51
#define RV_OPC_ITYPE    (0b0010011u) //  19
#define RV_OPC_BTYPE    (0b1100011u) //  99
#define RV_OPC_LOAD     (0b0000011u) //   3
#define RV_OPC_STORE    (0b0100011u) //  35
#define RV_OPC_LUI      (0b0110111u) //  55
#define RV_OPC_AUIP     (0b0010111u) //  23
#define RV_OPC_JAL      (0b1101111u) // 111
#define RV_OPC_JALR     (0b1100111u) // 103
#define RV_OPC_FENCE    (0b0001111u) //  15
#define RV_OPC_CSRENV   (0b1110011u) // 115

#define RV_FUNC_ADD         (0b000u)
#define RV_FUNC_SLT         (0b010u)
#define RV_FUNC_SLTU        (0b011u)
#define RV_FUNC_AND         (0b111u)
#define RV_FUNC_OR          (0b110u)
#define RV_FUNC_XOR         (0b100u)
#define RV_FUNC_SLL         (0b001u)
#define RV_FUNC_SRL         (0b101u)
#define RV_FUNC_SUB         (0b000u)
#define RV_FUNC_SRA         (0b101u)

#define RV_FUNC_ADDI        (0b000u)
#define RV_FUNC_SLTI        (0b010u)
#define RV_FUNC_SLTIU       (0b011u)
#define RV_FUNC_XORI        (0b100u)
#define RV_FUNC_ORI         (0b110u)
#define RV_FUNC_ANDI        (0b111u)
#define RV_FUNC_SLLI        (0b001u)
#define RV_FUNC_SHIFT_RIGHT (0b101u)

#define RV_FUNC_BEQ         (0b000u)
#define RV_FUNC_BNE         (0b001u)
#define RV_FUNC_BLT         (0b100u)
#define RV_FUNC_BGE         (0b101u)
#define RV_FUNC_BLTU        (0b110u)
#define RV_FUNC_BGEU        (0b111u)

#define RV_FUNC2_ARITH0 (0b0000000u)
#define RV_FUNC2_ARITH1 (0b0100000u)
#define RV_FUNC2_SLLI   (0b0000000u)
#define RV_FUNC2_SRLI   (0b0000000u)
#define RV_FUNC2_SRAI   (0b0100000u)

#define RV_WIDTH_BYTE        (0b00u)
#define RV_WIDTH_HALF        (0b01u)
#define RV_WIDTH_WORD        (0b10u)
#define RV_WIDTH_DOUBLE      (0b11u)
