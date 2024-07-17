#include "SymbolicExpression.h"
#include "SymbolicManager.h"
#include <fmt/core.h>
#include <fmt/ostream.h>


std::string to_string(SymbolicOperationType type)
{
    switch(type)
    {
        case SymbolicOperationType::sym_add   : return "+"; 
        case SymbolicOperationType::sym_sub   : return "-"; 
        case SymbolicOperationType::sym_shl   : return "<<"; 
        case SymbolicOperationType::sym_shra  : return ">>a "; 
        case SymbolicOperationType::sym_shrl  : return ">>l "; 
        case SymbolicOperationType::sym_xor   : return "^"; 
        case SymbolicOperationType::sym_and   : return "&"; 
        case SymbolicOperationType::sym_or    : return "|"; 
        case SymbolicOperationType::sym_concat: return "||"; 
        case SymbolicOperationType::sym_leak  : return "leak";
        default:
            assert(false);
            return "";
    }
}

Symbol::Symbol(std::shared_ptr<SymbolicManager> _manager, std::unique_ptr<PropVarSet> _propVarSet, size_t _id, std::string _name)
    : SymbolicExpression(_manager, std::move(_propVarSet)), id(_id), name(_name)
{
};

void Symbol::write(std::ostream &strm) const
{
    fmt::print(strm, "{}", name);
}

void SymbolicOperation::write(std::ostream &strm) const
{
    // TODO remove leak case
    if (type == SymbolicOperationType::sym_leak) {
        fmt::print(strm, "leak(");
        bool first = true;
        for (auto const& child : children) {
            if (child == nullptr)
                continue;
            if (!first)
                fmt::print(strm, ", ");
            child->write(strm);
            first = false;
        }
        fmt::print(strm, ")");
    } else {
        assert(children.size() > 0);
        if (children.size() == 1) {
            fmt::print(strm, "{}", to_string(type));
            children[0]->write(strm);
        } else {
            fmt::print(strm, "(");
            bool first = true;
            for (auto const& child : children) {
                if (child == nullptr)
                    continue;
                if (!first)
                    fmt::print(strm, " {} ", to_string(type));
                child->write(strm);
                first = false;
            }
            fmt::print(strm, ")");
        }
    }
}

void Symbol::constructPropVarSet(SatSolver* solver)
{
    propVarSet = std::make_unique<PropVarSet>(solver, lidx_t(id));
}

void SymbolicOperation::constructPropVarSet(SatSolver* solver)
{
    propVarSet = nullptr;
    if (type == SymbolicOperationType::sym_xor) {
        for (const auto& child : children) {
            if (child == nullptr)
                continue;
            if (propVarSet == nullptr) {
                propVarSet = child->propVarSet->clone();
            } else {
                propVarSet = *propVarSet ^ *child->propVarSet;
            }
        }
    } else {
        for (const auto& child : children) {
            if (child == nullptr)
                continue;
            if (propVarSet == nullptr) {
                propVarSet = +(*child->propVarSet);
            } else {
                propVarSet = *propVarSet ^ *(+(*child->propVarSet));
            }
        }
    }
    assert(propVarSet != nullptr);
}

std::shared_ptr<SymbolicExpression> SymbolicExpression::operator ^(SymbolicExpression& other) {
    // TODO more efficient
    std::vector<std::shared_ptr<SymbolicExpression>> vec;
    vec.push_back(shared_from_this());
    vec.push_back(other.shared_from_this());
    return manager->make_operation_symbol(SymbolicOperationType::sym_xor, vec);
}

std::shared_ptr<SymbolicOperation> makeSymbolicOperation(SymbolicOperationType type, std::vector<std::shared_ptr<SymbolicExpression>> children)
{
    assert(children.size() > 0);
    std::unique_ptr<PropVarSet> pvs;
    std::shared_ptr<SymbolicManager> manager;

    if (type == SymbolicOperationType::sym_xor) {
        for (const auto& child : children) {
            if (child == nullptr) {
                continue;
            }
            manager = child->manager;
//            if (pvs == nullptr) {
//                pvs = child->propVarSet->clone();
//            } else {
//                pvs = *pvs ^ *child->propVarSet;
//            }
        }
    } else {
        for (const auto& child : children) {
            if (child == nullptr) {
                continue;
            }
            manager = child->manager;
//            if (pvs == nullptr) {
//                pvs = +(*child->propVarSet);
//            } else {
//                pvs = *pvs ^ *(+(*child->propVarSet));
//            }
        }
    }
//    if (pvs == nullptr)
//        return nullptr;
    if (manager == nullptr)
        return nullptr;
    if (type == SymbolicOperationType::sym_leak) {
        return std::make_shared<SymbolicLeak>(manager, std::move(pvs), children);
    } else {
        return std::make_shared<SymbolicOperation>(manager, std::move(pvs), type, children);
    }
}

std::unique_ptr<PropVarSet> makeNonlinearCombination(const std::vector<std::shared_ptr<SymbolicExpression>>& children, std::vector<var_t>& bias_vars)
{
    assert(children.size() > 0);
    std::unique_ptr<PropVarSet> pvs;
    std::vector<std::unique_ptr<PropVarSet>> pvss;
    for (const auto& child : children) {
        if (child == nullptr)
            continue;
        var_t bias_var;
        pvss.push_back(child->propVarSet->bias(bias_var));
        bias_vars.push_back(bias_var);
    }
    // TODO more efficient
    for (const auto& p : pvss) {
        if (pvs == nullptr) {
            pvs = p->clone();
        } else {
            pvs = *pvs ^ *p;
        }
    }
    return pvs;
}

void SymbolicLeak::write(std::ostream &strm) const
{
    strm << "leak[";
    fmt::print(strm, "0x{:08x}", pc);
    strm << ",";
    strm << manager->get_contract_location_str(contract_location);
    strm << "](";
    bool first = true;
    for (auto const& child : children) {
        if (!first)
            fmt::print(strm, ", ");
        if (child == nullptr)
            fmt::print(strm, "_");
        else
            child->write(strm);
        first = false;
    }
    fmt::print(strm, ")");
}

std::ostream& operator<<(std::ostream &strm, const SymbolicExpression& sym) {
    sym.write(strm);
    return strm;
}
