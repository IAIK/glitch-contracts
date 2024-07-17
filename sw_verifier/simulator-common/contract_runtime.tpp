#pragma once
#include "contract_runtime.h"
#include "assert.h"
#include "common.h"


template<size_t W>
bv<W> __read_mem(ReadKind, const bv<32>& addr)
{
    assert(slice(addr, 1, 0) == bv_const(0b00, 2));
    static_assert(W == 8 || W == 16 || W == 32 || W == 64);
    if (mmio_rng_enabled && addr == mmio_rng_addr) {
        DEBUG(8) << "LOAD mem[" << bv_repr(addr) << "] (= RAND)" << std::endl;
        return globalSymbolicManager->make_mask_symbol<W>(W);
    }
    const auto& data = slice(simulator_memory[addr], W-1, 0);
    DEBUG(8) << "LOAD mem[" << bv_repr(addr) << "] (= " << bv_repr(data) << ")" << std::endl;
    return data;
}

template<size_t W>
bool __write_mem(WriteKind, const bv<32>& addr, const bv<W>& data)
{
    if constexpr (W == 32) {
        assert(slice(addr, 1, 0) == bv_const(0b00, 2));
        DEBUG(8) << "STORE mem[" << bv_repr(addr) << "] (= " << bv_repr(data) << ")" << std::endl;
        simulator_memory[addr] = data;
        return true;
    }
    INVALID_OPCODE();
}

template<size_t W>
bool __write_mem_bits(WriteKind, const bv<32>& addr, const bv<W>& data)
{
    if constexpr (W == 32) {
        assert(slice(addr, 1, 0) == bv_const(0b00, 2));
        DEBUG(8) << "STORE BIT mem[" << bv_repr(addr) << "] (= " << bv_repr(data) << ")" << std::endl;
        simulator_memory[addr] = simulator_memory[addr] | data;
        return true;
    }
    INVALID_OPCODE();
}
