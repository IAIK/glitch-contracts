#pragma once

#include <inttypes.h>
#include <cstddef>
#include <vector>
#include <cassert>
#include <map>
#include <fstream>
#include <iostream>
#include "SatSolver.h"

extern "C" {
#include "ipasir.h"
}

class SatSolverIPASIR : public SatSolver {
protected:
    /// IPASIR solver object with generic (void*) type
    void* m_solver;

    virtual void add_internal(var_t i)
    {
        ipasir_add(m_solver, i);
    }

public:
    SatSolverIPASIR() : SatSolver(), m_solver(ipasir_init()){}

    virtual ~SatSolverIPASIR() { ipasir_release(m_solver); }

    virtual state_t check()
    {
#ifdef DEBUG_SOLVER
        debug_file << "# learned clauses:" << std::endl;
        ipasir_set_learn(m_solver, this, 10, [](void* satsolver, int*clause) {
            DEBUG(15) << "learned: ";
            do {
                ((SatSolverIPASIR*)satsolver)->debug_file << *clause << ' ';
                DEBUG(8) << *clause << ' ';
            } while (*(clause++) != 0);
            ((SatSolverIPASIR*)satsolver)->debug_file << std::endl;
            DEBUG(8) << std::endl;
        });
#endif
        int res = ipasir_solve(m_solver);
        assert(res == state_t::STATE_SAT || res == state_t::STATE_UNSAT);
#ifdef DEBUG_SOLVER
        ipasir_set_learn(m_solver, nullptr, 0, nullptr);
#endif
        return (state_t)res;
    }
    
    virtual var_t value(var_t v) {
        return (var_t)ipasir_val(m_solver, v);
    }

    virtual bool failed(var_t v) {
        return ipasir_failed(m_solver, v);
    }
};
