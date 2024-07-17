#include "contract_runtime.h"

std::map<bv<32>, bv<32>> simulator_memory;

bool mmio_rng_enabled;
bv<32> mmio_rng_addr;

std::shared_ptr<SymbolicManager> globalSymbolicManager;

bool error(char const * msg)
{
  puts(msg);
  INVALID_OPCODE();
}
