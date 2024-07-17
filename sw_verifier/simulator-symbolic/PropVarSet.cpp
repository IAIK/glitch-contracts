//
// Created by vhadzic on 25.09.20.
//

#include "PropVarSet.h"

#include <memory>
#include <cassert>
#include <iostream>
#include "SatSolver.h"

//#define SPACING ("                            ")
#define SPACING ("")

PropVarSet::PropVarSet(SatSolver* sol, lidx_t id) :
  solver(sol), fresh_biased(false)
{
    DEBUG(8) << "PropVarSet::PropVarSet id=" << id << std::endl;
    vars[id] = ONE;
}

PropVarSet::PropVarSet(SatSolver* sol, bool _fresh_biased) :
  solver(sol), fresh_biased(_fresh_biased)
{
    DEBUG(8) << "PropVarSet::PropVarSet biased=" << _fresh_biased << std::endl;
}


std::unique_ptr<PropVarSet> PropVarSet::clone() const
{
    return std::unique_ptr<PropVarSet>(new PropVarSet(*this)); // TODO improve
}

std::unique_ptr<PropVarSet> operator^(PropVarSet& first, PropVarSet& second)
{
    PropVarSet* p = &first;
    assert(p != nullptr);
    PropVarSet* q = &first;
    assert(q != nullptr);
    assert(first.solver == second.solver);
    PropVarSet res(first);
#ifdef OPT_FRESH_BIASED
    if ((res.fresh_biased = first.fresh_biased && second.fresh_biased)) {
        first.fresh_biased  = false;
        second.fresh_biased = false;
    }
#endif
    for (auto const& [label, s2] : second.vars) {
        auto const& s1 = first[label];
        if (s1 == s2)                  // s1 in {0, 1, v}, s2 in {1, v}
            res.vars.erase(label);
        else if (s1 == -s2)            // s1 in {0, 1, v}, s2 in {1, v}, s1!=s2
            res.vars[label] = ONE;
        else if(s1 == ZERO)            // s1 in {0, 1, v}, s2 in {1, v}, s1!=s2, s1!=-s2
            res.vars[label] = s2;
        else if(s1 == ONE)             // s1 in {1, v}, s2 in {1, v}, s1!=s2, s1!=-s2
            res.vars[label] = -s2;
        else if(s2 == ONE)             // s1 in {v}, s2 in {1, v}, s1!=s2, s1!=-s2
            res.vars[label] = -s1;
        else                           // s1 in {v}, s2 in {v}, s1!=s2, s1!=-s2
            res.vars[label] = res.solver->make_xor(s1, s2);
    }
    DEBUG(9) << "CALC " << &first << " ^ " << &second << " = " << &res << std::endl << SPACING;
    return std::unique_ptr<PropVarSet>(new PropVarSet(res)); // TODO improve
}

std::unique_ptr<PropVarSet> operator+(PropVarSet& pvs)
{
    var_t bias_var;
    return pvs.bias(bias_var);
}

std::unique_ptr<PropVarSet> PropVarSet::bias(var_t& bias_var)
{
    PropVarSet res(*this);
#ifdef OPT_FRESH_BIASED
    // TODO set bias_var
    res.fresh_biased = true;
    if (res.fresh_biased) {
        res.fresh_biased = false;
        return std::unique_ptr<PropVarSet>(new PropVarSet(res)); // TODO improve
    }
#endif
    //if (!res.vars.empty()) {
    bias_var = res.solver->new_var();
    for (auto& [label, solver_var] : res.vars) {
        assert (solver_var != ZERO);
        solver_var = (solver_var == ONE) ? bias_var : res.solver->make_and(bias_var, solver_var);
    }
    //}
    DEBUG(9) << "CALC +" << this << " = " << &res << std::endl << SPACING;
    return std::unique_ptr<PropVarSet>(new PropVarSet(res)); // TODO improve
}

std::ostream& operator<<(std::ostream &strm, const PropVarSet* pvs) {
    strm << "{";
    if (pvs != nullptr) {
        bool first = true;
        for (auto const& [label, solver_var] : pvs->vars) {
            if (!first)
                strm << ", ";
            strm << label << ": ";
            if (solver_var == ONE) {
                strm << "T";
            } else {
                strm << solver_var;
            }
            first = false;
        }
    }
    strm << "}";
    return strm;
}
