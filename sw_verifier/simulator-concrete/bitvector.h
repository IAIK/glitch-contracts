#pragma once
#include <stdint.h>
#include <unistd.h>
#include <fmt/core.h>
#include <stdlib.h>


template <size_t N>
using bv = unsigned _BitInt(N);

template <size_t N>
using sbv = signed _BitInt(N);

template<size_t... Ns>
void leak(bv<Ns>... args) {
    // NOP
}

template <size_t N>
std::string _bv_to_hex(bv<N> b) {
    static_assert(N <= 64, "printing large bitvectors not implemented");
    return fmt::format("0x{:0{}x}", (uint64_t)b, (int)((N-1)/4+1)); // converted to nibble width
}

template<class T, class E, size_t end, size_t start>
E _slice(T b)
{
  static_assert(start <= end);
  return (b >> start) & ((1ull << (end-start+1)) - 1);
}

#define bool_to_bits(b) (b)
#define zero_extend_template(b, new_size) (zero_extend((b), (new_size)))
#define zero_extend(b, size) (bv<(size)>(b))
#define sign_extend(b, old_size, new_size) ((sbv<(new_size)>)(sbv<(old_size)>)(b))
#define signed_lower_than(a, b, size) ((sbv<(size)>)(a) < (sbv<(size)>)(b))
#define shift_right_arith(b, amm, size) ((sbv<(size)>)(b) >> (amm))
#define bv_to_dec(b) (std::to_string((uint64_t)(b)).c_str())
#define bv_to_hex(b) (_bv_to_hex(b).c_str())
#define bv_repr(b) (bv_to_hex(b))
#define bv_cast_concrete(b) (b)
#define slice(b, end, start) (_slice<typeof(b), bv<((end)-(start)+1)>, (end), (start)>((b)))
#define bv_const(value, size) (bv<size>(value))

#define INVALID_OPCODE() do {printf("invalid opcode, exiting...\n"); exit(99);} while (0)
