#include <fmt/core.h>
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include "contract_runtime.h"

void debug_mem()
{
    for (auto const& [key, val] : simulator_memory)
    {
        std::cout << (uint64_t)key << ": " << (uint64_t)val << std::endl;
    }
}
