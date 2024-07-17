#pragma once

#include <inttypes.h>
#include <cstddef>
#include <vector>
#include <cassert>
#include <map>
#include <fstream>
#include "SatSolver.h"

extern "C" {
#include <kissat.h>
}

class SatSolverKissat : public SatSolver {
protected:
    static constexpr uint32_t INITIAL_RESERVED = 2<<20;

    kissat* kissat;

    inline void set_reserved(uint32_t res)
    {
        reserve_count = res;
        kissat_reserve(kissat, res);
    }

    virtual void add_internal(var_t i)
    {
        kissat_add(kissat, i);
    }

public:
    SatSolverKissat() : SatSolver(), kissat(kissat_init())
    {
        set_reserved(INITIAL_RESERVED);
    }

    virtual ~SatSolverKissat() { kissat_release(kissat); }

    virtual state_t check()
    {
        int res = kissat_solve(kissat);
        assert(res == state_t::STATE_SAT || res == state_t::STATE_UNSAT);
        return (state_t)res;
    }

    bool supports_incremental()
    {
        int res = kissat_solve(kissat);
        assert(res == state_t::STATE_SAT || res == state_t::STATE_UNSAT);
        return true;
    }
    
    virtual var_t value(var_t v) {
        return (var_t)kissat_value(kissat, v);
    }

    virtual bool failed(var_t v) { return true; }
};
