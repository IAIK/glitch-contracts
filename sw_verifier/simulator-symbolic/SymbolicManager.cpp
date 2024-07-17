#include <set>
#include <stack>
#include "SymbolicManager.h"

#ifdef BACKEND_API_KISSAT
#define INCREMENTAL_ERROR_SEARCH (0)
#else
#define INCREMENTAL_ERROR_SEARCH (1)
#endif


SymbolicManager::SymbolicManager(size_t _order) : order(_order)
{
    contract_location_itos.push_back("");
}

SymbolicManager::~SymbolicManager()
{
    delete solver;
}

void extract_linear(std::set<SymbolicExpression*>& set, std::shared_ptr<SymbolicExpression> sym) {
    if (sym->isOp()) {
        auto opSym = std::static_pointer_cast<SymbolicOperation>(sym);
        if (opSym->type == SymbolicOperationType::sym_xor) {
            //printf("%p ", opSym.get());
            set.insert(opSym.get());
        } else {
            for (auto& child : opSym->children)
                if (child != nullptr)
                    extract_linear(set, child);
        }
    } else {
        //printf("%p ", sym.get());
        set.insert(sym.get());
    }
}

void SymbolicManager::remove_duplicate_leaks(size_t horizon)
{
    std::vector<std::vector<SymbolicExpression*>> linear_leak_components;
    linear_leak_components.reserve(leaks.size());
    for(const auto& leak : leaks) {
        //std::cout << *leak << ' ';
        std::set<SymbolicExpression*> lin_components;
        extract_linear(lin_components, leak);
        //std::sort(lin_components.begin(), lin_components.end());
        //std::cout << std::endl;
        //linear_leak_components.push_back(lin_components);
        std::vector<SymbolicExpression*> vc(lin_components.begin(), lin_components.end());
        linear_leak_components.push_back(vc);
    }
    for (size_t i = 0; i < leaks.size(); i++) {
        for (size_t j = i+1; j < std::min(i+horizon, leaks.size()); j++) {
            bool order = (linear_leak_components[i].size() > linear_leak_components[j].size());
            size_t long_idx = order ? i : j;
            size_t short_idx = order ? j : i;
            std::vector<SymbolicExpression*>& long_list = linear_leak_components[long_idx];
            std::vector<SymbolicExpression*>& short_list = linear_leak_components[short_idx];
            size_t lidx = 0;
            bool is_subset = true;
            for (const auto& s : short_list) {
                while (true) {
                    if (lidx == long_list.size()) {
                        is_subset = false;
                        break;
                    }
                    auto& l = long_list[lidx];
                    lidx++;
                    if (s == l) break;
                    if (s < l) {
                        is_subset = false;
                        break;
                    }
                }
                if (!is_subset) break;
            }
            if (is_subset) {
                //printf("%d is subset of %d\n", short_idx, long_idx);
                assert(short_idx < leaks.size());
                leaks.erase(leaks.begin()+short_idx);
                linear_leak_components.erase(linear_leak_components.begin()+short_idx);
                if (i >= short_idx) {
                    i--;
                    break; // abort loop as i was deleted
                } 
                if (j >= short_idx) j--;
            }
        }
    }
    assert(leaks.size() == linear_leak_components.size());

//    // prune unused elements from expression list
//    auto i = expressions.rbegin();
//    while ( i != expressions.rend() ) {
//        if ( i->use_count() == 1) {
//            i = decltype(i)(expressions.erase( std::next(i).base() ));
//        } else {
//            ++i;
//        } 
//    }
    //for (auto exp = expressions.rbegin(); exp != expressions.rend(); exp++)
    //{
    //    if (exp->use_count() == 1) {
    //        expressions.erase(exp);
    //    }
    //}
}

void SymbolicManager::constructPropVarSets()
{
    delete solver;
    solver = new SOLVER_CLASS();

    // TODO encode secrets and masks

    for(const auto& [secret_id, shares] : secrets) {
        for(const auto& share : shares) {
            share->constructPropVarSet(solver);
        }
    }

    for(const auto& mask : masks) {
        mask->constructPropVarSet(solver);
    }

    ///std::stack<std::shared_ptr<SymbolicExpression>> exprs(leaks.begin(), leaks.end());
    std::stack<std::shared_ptr<SymbolicExpression>>::container_type tmp_exprs(leaks.rbegin(), leaks.rend());
    std::stack<std::shared_ptr<SymbolicExpression>> exprs(std::move(tmp_exprs));

    std::cout << "recursive pvs construction started..." << std::endl;
    while (!exprs.empty()) {
        auto sym = exprs.top();

        //std::cout << "top: "<< *sym << std::endl;

        if (sym->isOp()) {
            auto opSym = std::static_pointer_cast<SymbolicOperation>(sym);
            bool done = true;
            for (auto& child : opSym->children) {
                if (child != nullptr && child->propVarSet == nullptr) {
                    //std::cout << "add child: " << *child << std::endl;
                    exprs.push(child);
                    done = false;
                }
            }
            if (done) {
                sym->constructPropVarSet(solver);
                exprs.pop();
            }
        } else {
            sym->constructPropVarSet(solver);
            exprs.pop();
        }
    }

//    std::cout << "expressions:" << std::endl;
//    for(const auto& expression : expressions) {
//        std::cout << "  - " << *expression << std::endl;
//    }
//    std::cout << "expressions end" << std::endl;
//
//    for(const auto& expression : expressions) {
//        std::cout << "constructPropVarSet " << *expression << std::endl;
//        expression->constructPropVarSet(solver);
//    }
}

int SymbolicManager::check_leaks()
{
    std::cout << "removing duplicate leaks..." << std::endl;
    remove_duplicate_leaks(100); // TODO command line option
    constructPropVarSets();
    if (leaks.size() == 0)
    {
        std::cout << "WARNING: no leaks were recorded, exiting..." << std::endl;
        return 20; // TODO...
    }

    DEBUG(4) << leaks.size() << " leaks recorded:" << std::endl;
    for(const auto& leak : leaks) {
        DEBUG(4) << *leak << ' ' << &leak->getPropVarSet() << std::endl;
    }
    DEBUG(4) << std::endl;

    // combine all leaks, limit to k (=order) leaks
    std::cout << "encoding " << order << "-out-of-n..." << std::endl;
    std::vector<var_t> leak_vars;
    leak_vars.reserve(masks.size());
    std::unique_ptr<PropVarSet> lpvs = makeNonlinearCombination(leaks, leak_vars);
    PropVarSet& leak_pvs = *lpvs;
    solver->make_atmost_k_of_n(order, leak_vars);

    std::cout << "encoding secrets as all-or-nothing..." << std::endl;
    std::vector<var_t> pos_secret_vars;
    std::vector<var_t> neg_secret_vars;
    pos_secret_vars.reserve(secret_shares.size());
    neg_secret_vars.reserve(secret_shares.size());
    for(const auto& [secret_id, shares] : secrets) {
        DEBUG(4) << "encoding secret " << secret_id << ", shares: ";
        bool fixed_pos_exists = false;
        bool fixed_neg_exists = false;
        std::vector<var_t> non_fixed_shares_pos;
        std::vector<var_t> non_fixed_shares_neg;
        for(const auto& share : shares) {
            var_t share_var = leak_pvs[share->getId()];
            DEBUG(4) << share->getId() << "=" << share_var << ", ";
            if (share_var == ZERO) {
                fixed_neg_exists = true;
            } else  if (share_var == ONE) {
                fixed_pos_exists = true;
            } else {
                non_fixed_shares_pos.push_back(share_var);
                non_fixed_shares_neg.push_back(-share_var);
            }
        }
        DEBUG(4) << std::endl;
        if ((fixed_pos_exists && fixed_neg_exists)) {
            std::cout << "secret is always partially leaked -> no full leak possible" << std::endl;
            continue;
        }
        if (non_fixed_shares_pos.size() == 0) {
            if (fixed_pos_exists) {
                std::cout << "secret " << secret_id << " leaks" << std::endl;
                return 10; // TODO...
            } else {
                std::cout << "secret " << secret_id << " cannot be leaked, excluding from analysis..." << std::endl;
                continue;
            }
        }
        std::vector<var_t> all_neg_or_pos;
        if (!fixed_neg_exists) {
            auto all_pos = solver->make_implies_all(non_fixed_shares_pos);
            pos_secret_vars.push_back(all_pos);
            all_neg_or_pos.push_back(all_pos);
        }
        if (!fixed_pos_exists) {
            auto all_neg = solver->make_implies_all(non_fixed_shares_neg);
            neg_secret_vars.push_back(all_neg);
            all_neg_or_pos.push_back(all_neg);
        }
        assert(all_neg_or_pos.size() > 0);
        solver->add_clause(all_neg_or_pos);
    }
    if (pos_secret_vars.size() == 0) {
        std::cout << "no secret can be fully leaked" << std::endl;
        return 20; // TODO...
    }

    // assume at least one secret to be leaked
    solver->add_clause(pos_secret_vars);

    // assume not masked
    for(const auto& mask : masks) {
        var_t mask_var = leak_pvs[mask->getId()];
        if (mask_var == ONE) {
            std::cout << "mask is always in pvs -> no leak possible" << std::endl;
            return 20; // TODO...
        } else if (mask_var != ZERO) {
            solver->add_clause(-mask_var);
        }
    }

    while (true) {
        std::cout << "Solving with " << solver->get_var_count()
                << " variables and " << solver->get_clause_count()
                << " clauses" << std::endl;

        SatSolver::state_t res = solver->check();

        if (res == SatSolver::state_t::STATE_SAT) {
            std::cout << "SAT -> insecure" << std::endl;
            std::cout << "leaks used:" << std::endl;
            int last_active_lidx = 0;
            for (int lidx = 0; lidx < leak_vars.size(); lidx++) {
                const auto& leak_var = leak_vars[lidx];
                if (solver->value(leak_var) <= 0)
                    continue;
                const auto& leak = leaks[lidx];
                last_active_lidx = lidx;
                std::cout << " - leak " << lidx << " (encoded as " << leak_var << "): " << std::endl;
                print_active_labels(leak->getPropVarSet());
                std::cout << " " << *leaks[lidx] << " " << &leaks[lidx]->getPropVarSet() << std::endl;
            }
            std::cout << "\njoint leak: ";
            print_active_labels(leak_pvs);
            std::cout << "\n" << std::endl;
            for (int lidx = 1; lidx <= solver->get_var_count(); lidx++) {
                DEBUG(20) << "label " << lidx << " = " << solver->value((var_t)lidx) << std::endl;
            }
            if (INCREMENTAL_ERROR_SEARCH) { // TODO add feature-flag / command-line-param to disable
                std::cout << "\n+++ retrying with leaks before leak " << last_active_lidx << std::endl;
                for (int lidx = last_active_lidx; lidx < leak_vars.size(); lidx++) {
                    const auto& leak_var = leak_vars[lidx];
                    solver->add_clause(-leak_var);
                }
                continue;
            }
        } else {
            std::cout << "UNSAT -> secure" << std::endl;
            std::cout << "\nlabels used: " << std::endl;
            for (int lidx = 1; lidx <= solver->get_var_count(); lidx++) {
                if (solver->failed((var_t)lidx)) {
                    std::cout << lidx << std::endl;
                }
            }
        }
        return res;
    }
}

void SymbolicManager::print_active_labels(const PropVarSet& pvs)
{
    std::cout << "{";
    bool first = true;
    for(const auto& [secret_id, shares] : secrets) {
        print_active_labels(pvs, shares, first);
    }
    print_active_labels(pvs, masks, first);
    std::cout << "}";
}

void SymbolicManager::print_active_labels(const PropVarSet& pvs, const std::vector<std::shared_ptr<Symbol>>& symbols, bool& first)
{
    for(const auto& sym : symbols) {
        var_t share_var = pvs[sym->getId()];
        if (share_var == ONE || (share_var != ZERO && solver->value(share_var) > 0)) {
            if (!first)
                std::cout << " ";
            std::cout << sym->getName();
            first = false;
        }
    }
}

std::string secret_name(size_t start, size_t end, size_t share_id) {
    //return fmt::format("{}{}", (char)('A'+(start/32)), share_id);
    return fmt::format("{}{}", (char)('A'+share_id), start/16);
}
