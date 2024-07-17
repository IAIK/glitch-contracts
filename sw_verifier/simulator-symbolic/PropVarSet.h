#pragma once
//
// Created by vhadzic on 25.09.20.
//
#include <inttypes.h>
#include <map>
#include <memory>
#include "common.h"

class SatSolver;

class PropVarSet {
private:
    std::map<lidx_t, var_t> vars;
    bool fresh_biased = false;
    SatSolver* solver;
    PropVarSet(const PropVarSet&) = default;
    PropVarSet(SatSolver* sol, bool _fresh_biased);

public:
    PropVarSet(SatSolver* sol, lidx_t id);
    PropVarSet() = delete;
    std::unique_ptr<PropVarSet> clone() const;

    var_t operator[](const lidx_t& pos) const
    {
        auto v = vars.find(pos);
        return (v == vars.end()) ? ZERO : v->second;
    }

    size_t size() const { return vars.size(); }
    // creates an xor of two variable sets
    friend std::unique_ptr<PropVarSet> operator^(PropVarSet& first, PropVarSet& second);
    // creates a biased copy of a variable set
    friend std::unique_ptr<PropVarSet> operator+(PropVarSet& obj);
    // creates a biased copy of a variable set and sets bias_var to the solver variable
    std::unique_ptr<PropVarSet> bias(var_t& bias_var);

    friend std::ostream& operator<<(std::ostream &strm, const PropVarSet* pvs);
};
