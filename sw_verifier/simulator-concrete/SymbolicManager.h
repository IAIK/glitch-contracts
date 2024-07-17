#pragma once
#include <inttypes.h>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <cassert>
#include "contract_runtime.h"
#include "bitvector.h"

class SymbolicManager 
{
public:
    SymbolicManager(size_t _order) {};
    int check_leaks() { return 0; };
    template <size_t N>
    bv<N> make_secret_symbol(size_t start, size_t end)
    {
        assert(false);
        return bv<N>(0);
    }

    template <size_t N>
    bv<N> make_mask_symbol()
    {
        assert(false);
        return bv<N>(0);
    }
};
