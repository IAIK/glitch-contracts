#include <fmt/core.h>
#include <iostream>
#include <string>
#include <string_view>
#include "contract_runtime.h"
#include "SymbolicManager.h"
#include "bitvector.h"

template
std::ostream& operator<< <32>(std::ostream &strm, const bv_frag<32> &a);

void debug_mem()
{
    for (auto const& [key, val] : simulator_memory)
    {
        std::cout << key << ": " << val << std::endl;
    }
}

void leak_vector(const std::string& contract_location, bv<32> pc, const std::vector<std::shared_ptr<SymbolicExpression>>& vec) {
    assert(vec.size() > 0);
    std::shared_ptr<SymbolicExpression> leak_sym = globalSymbolicManager->make_leak_operation_symbol(contract_location, pc.to_uint(), vec);
    if (leak_sym) {
        DEBUG(7) << *leak_sym << " " << &leak_sym->getPropVarSet() << std::endl;
    }
}

// TODO unify with other code
std::shared_ptr<SymbolicExpression> concat_symbols(const std::vector<std::shared_ptr<SymbolicExpression>> symbols) {
    if (symbols.size() == 0)
        return nullptr;
    if (symbols.size() == 1)
        return symbols[0];
    assert(symbols[0] != nullptr);
    std::shared_ptr<SymbolicManager> m = symbols[0]->get_manager();
    return m->make_operation_symbol(SymbolicOperationType::sym_concat, symbols);
}

