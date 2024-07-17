#pragma once
#include <inttypes.h>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
#include <map>
#include <fstream>
#include <numeric>
#include "SatSolver.h"
#include "PropVarSet.h"
#include "SymbolicExpression.h"
//#include "contract_runtime.h"
#include "bitvector.h"

#ifdef BACKEND_API_KISSAT
#include "SatSolverKissat.h"
#define SOLVER_CLASS SatSolverKissat
#else
#include "SatSolverIPASIR.h"
#define SOLVER_CLASS SatSolverIPASIR
#endif

namespace std {
template <>
struct hash<std::vector<std::shared_ptr<SymbolicExpression>>> {
  size_t operator()(const std::vector<std::shared_ptr<SymbolicExpression>> &v) const {
    size_t hash = 0;
    for (auto& e : v) {
        hash += size_t(e.get());
    }
    return hash;
  }
};
}  // namespace std

std::string secret_name(size_t start, size_t end, size_t share_id);

class SymbolicManager 
    : public std::enable_shared_from_this<SymbolicManager>
{
private:
    size_t order = 0;
    lidx_t label_count = 0;
    bool expanded_sharing = false;
    std::map<size_t, std::vector<std::shared_ptr<Symbol>>> secrets;
    std::map<size_t, std::vector<std::shared_ptr<SymbolicExpression>>> secret_shares;
    std::map<size_t, size_t> secret_shares_count;
    std::vector<std::shared_ptr<Symbol>> masks;
    std::vector<std::shared_ptr<SymbolicExpression>> leaks;
    SatSolver* solver = new SOLVER_CLASS();

    std::unordered_set<std::vector<std::shared_ptr<SymbolicExpression>>> leak_cache;
    std::map<std::vector<std::shared_ptr<SymbolicExpression>>, std::shared_ptr<SymbolicOperation>> fragment_cache;

    std::map<std::string, uint> contract_location_stoi;
    std::vector<std::string> contract_location_itos;

public:
    SymbolicManager(size_t _order);
    ~SymbolicManager();
    void remove_duplicate_leaks(size_t horizon);
    void constructPropVarSets();
    int check_leaks();
    void print_active_labels(const PropVarSet& pvs);
    void print_active_labels(const PropVarSet& pvs, const std::vector<std::shared_ptr<Symbol>>& symbols, bool& first);

    const std::string& get_contract_location_str(int loc) {
        return contract_location_itos[loc];
    } 
    
#ifdef DEBUG_SOLVER
    void set_debug_file(std::ofstream& d) { solver->set_debug_file(d); }
#endif

    void create_secret_sharing(size_t start, size_t end, size_t shares) {
        if (expanded_sharing) {
            auto sym = std::make_shared<Symbol>(shared_from_this(),
                                                std::make_unique<PropVarSet>(solver, label_count),
                                                label_count,
                                                secret_name(start, end, 0));
            label_count++;
            secrets[start].push_back(sym);
            //expressions.push_back(sym);
            for (int share_id = 0; share_id < order + 1; share_id++) {
                std::shared_ptr<SymbolicExpression> share;
                if (share_id == order) {
                    share = sym;
                } else {
                    size_t mask_id = masks.size();
                    auto mask = std::make_shared<Symbol>(shared_from_this(),
                                                        std::make_unique<PropVarSet>(solver, label_count),
                                                        label_count,
                                                        fmt::format("M{}", mask_id));
                    label_count++;
                    masks.push_back(mask);
                    //expressions.push_back(sym);
                    share = *sym ^ *mask;
                    sym = mask;
                }
                secret_shares[start].push_back(share);
                //expressions.push_back(share);
            }
        } else {
            for (int share_id = 0; share_id < order + 1; share_id++) {
                auto sym = std::make_shared<Symbol>(shared_from_this(),
                                                    std::make_unique<PropVarSet>(solver, label_count),
                                                    label_count,
                                                    secret_name(start, end, share_id));
                label_count++;
                secret_shares[start].push_back(sym);
                secrets[start].push_back(sym);
                //expressions.push_back(sym);
            }
        }
    }

    template <size_t N>
    bv<N> make_secret_symbol(size_t start, size_t end)
    {
        // TODO check if size of all shares is equal?
        if (secret_shares.find(start) == secret_shares.end()) {
            create_secret_sharing(start, end, order + 1);
        }
        int share_id = secret_shares_count[start]++;
        assert(share_id < secret_shares[start].size());
        auto sym = secret_shares[start][share_id];
        std::cout << "new secret symbol: " << *sym << " " << &sym->getPropVarSet() << std::endl;
        return bv<N>(static_pointer_cast<SymbolicExpression>(sym), end-start+1);
    }

    template <size_t N>
    bv<N> make_mask_symbol(size_t size)
    {
        size_t mask_id = masks.size();
        auto sym = std::make_shared<Symbol>(shared_from_this(), std::make_unique<PropVarSet>(solver, label_count), label_count, fmt::format("M{}", mask_id));
        label_count++;
        masks.push_back(sym);
        //expressions.push_back(sym);
        std::cout << "new mask symbol: " << *sym << " " << &sym->getPropVarSet() << std::endl;
        return bv<N>(static_pointer_cast<SymbolicExpression>(sym), size);
    }

    std::shared_ptr<SymbolicOperation> make_operation_symbol(SymbolicOperationType type, std::vector<std::shared_ptr<SymbolicExpression>> children)
    {
        assert(type != SymbolicOperationType::sym_leak);

        std::vector<std::shared_ptr<SymbolicExpression>> ordered_children(children);
        if (type == SymbolicOperationType::sym_concat) {
            ordered_children.erase( remove( ordered_children.begin(), ordered_children.end(), nullptr ), ordered_children.end() );
            std::sort(ordered_children.begin(), ordered_children.end());
            if (fragment_cache.contains(ordered_children)) {
                DEBUG(15) << "fragment is a duplicate" << std::endl;
                return fragment_cache[ordered_children];
            }
        }

        auto sym = makeSymbolicOperation(type, children);
        if (!sym)
            return nullptr;


        if (type == SymbolicOperationType::sym_concat) {
            fragment_cache[ordered_children] = sym;
        }

        //expressions.push_back(sym);
        return sym;
    }

    std::shared_ptr<SymbolicOperation> make_leak_operation_symbol(const std::string& contractLocation, uint32_t pc, std::vector<std::shared_ptr<SymbolicExpression>> children)
    {
        std::vector<std::shared_ptr<SymbolicExpression>> ordered_children(children);
        ordered_children.erase( remove( ordered_children.begin(), ordered_children.end(), nullptr ), ordered_children.end() );
        std::sort(ordered_children.begin(), ordered_children.end());
        if (leak_cache.contains(ordered_children)) {
            DEBUG(15) << "leak is a duplicate" << std::endl;
            return nullptr;
        }
        leak_cache.insert(ordered_children);

        std::shared_ptr<SymbolicLeak> sym = std::static_pointer_cast<SymbolicLeak>(makeSymbolicOperation(SymbolicOperationType::sym_leak, children));
        int contract_location_i = contract_location_stoi[contractLocation];
        if (contract_location_i == 0) {
            contract_location_i = contract_location_itos.size();
            contract_location_stoi[contractLocation] = contract_location_i;
            contract_location_itos.push_back(contractLocation);
        }
        if (!sym)
            return nullptr;
        sym->set_contract_location(contract_location_i);
        sym->set_pc(pc);
        //expressions.push_back(sym);
        DEBUG(7) << "leak " << leaks.size() << ": ";
        leaks.push_back(sym);
        return sym;
    }
};
