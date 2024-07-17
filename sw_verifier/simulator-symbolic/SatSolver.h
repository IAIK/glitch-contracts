#pragma once

#include <inttypes.h>
#include <cstddef>
#include <vector>
#include <cassert>
#include <map>
#include <fstream>
#include "common.h"

class SatSolver {
protected:
    uint32_t var_count = 0;
    uint32_t clause_count = 0;
    uint32_t reserve_count = 0;
#ifdef OPT_SAT_ENCODING_CACHE
    std::map<std::tuple<int32_t, int32_t>, int32_t> linear_cache;
    std::map<std::tuple<int32_t, int32_t>, int32_t> nonlinear_cache;
#endif
#ifdef DEBUG_SOLVER
    bool clause_open = false;
#endif

    virtual void add_internal(var_t i) = 0;

    inline void add(var_t i)
    {
#ifdef DEBUG_SOLVER
        clause_open = true;
#endif
        assert(i != 0);
        assert(i != ZERO);
        assert(i != ONE);
        add_internal(i);
#ifdef DEBUG_SOLVER
        debug_file << i << ' ';
        if (i == 0) debug_file << std::endl;
#endif
    }

    inline void end_clause()
    {
#ifdef DEBUG_SOLVER
        assert(clause_open);
        clause_open = false;
#endif
        add_internal((var_t)0);
#ifdef DEBUG_SOLVER
        debug_file << '0' << std::endl;
#endif
    }

    inline void add_clause(const int v1) {
        clause_count++;
        add((var_t)v1);
        end_clause();
    };
    
    inline void add_clause(const int v1, const int v2) {
        clause_count++;
        add((var_t)v1);
        add((var_t)v2);
        end_clause();
    };
    
    inline void add_clause(const int v1, const int v2, const int v3) {
        clause_count++;
        add((var_t)v1);
        add((var_t)v2);
        add((var_t)v3);
        end_clause();
    };

public:
    enum state_t {STATE_SAT = 10, STATE_UNSAT = 20, STATE_INPUT = 30};

#ifdef DEBUG_SOLVER
    std::ofstream debug_file;
#endif
#ifdef DEBUG_SOLVER
    void set_debug_file(std::ofstream& d) { debug_file = std::move(d); }
#endif
    SatSolver() { }
    virtual ~SatSolver() = default;
    inline uint32_t get_var_count() { return var_count; }
    inline uint32_t get_clause_count() { return clause_count; }
    var_t make_xor(var_t a, var_t b);
    var_t make_and(var_t a, var_t b);
    void make_atmost_k_of_n(size_t k, const std::vector<var_t>& xs);
    var_t make_implies_all(std::vector<var_t>& xs);
    virtual state_t check() = 0;
    virtual var_t value(var_t v) = 0;
    virtual bool failed(var_t v) = 0;

    inline var_t new_var()
    {
        var_count++;
        return (var_t)var_count;
    }

    inline var_t new_vars(const size_t cnt)
    {
        const int v = var_count;
        var_count = v + cnt;
        return (var_t)(v + 1);
    }
    
    inline void add_clause(const var_t v1) {
        clause_count++;
        add(v1);
        end_clause();
    };
    
    inline void add_clause(const var_t v1, const var_t v2) {
        clause_count++;
        add(v1);
        add(v2);
        end_clause();
    };
    
    inline void add_clause(const var_t v1, const var_t v2, const var_t v3) {
        clause_count++;
        add(v1);
        add(v2);
        add(v3);
        end_clause();
    };
    
    inline void add_clause(const std::vector<var_t>& vars) {
        clause_count++;
        for (auto i : vars)
           add(i);
        end_clause();
    };

};
