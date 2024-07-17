#pragma once
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <map>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <memory>

#include "common.h"
#include "../common/contract_runtime_common.h"
#include "bitvector.h"
#include "SymbolicManager.h"

extern std::shared_ptr<SymbolicManager> globalSymbolicManager;
extern bool mmio_rng_enabled;
extern bv<32> mmio_rng_addr;

#define init_register(regname, size, initial_val) do {regname = initial_val;} while (0)
#define read_mem(addr, width) (__read_mem<width*8>(data_read, (addr)))
#define fetch_instruction(addr, width) (__read_mem<width*8>(instruction_fetch, (addr))
#define write_mem(addr, width, data) (__write_mem<width*8>(data_write, (addr), (data)))

#if VERBOSITY <= 0
#define printf(args...)
#endif

// -----------------------
// implementation stuff (should not be accessed by contract)
// -----------------------

void __register_register(char* loc, size_t size, std::string name);

enum ReadKind {
  data_read,
  instruction_fetch
};

enum WriteKind {
  data_write
};

template<size_t W>
bv<W> __read_mem(ReadKind, const bv<32>& addr);

template<size_t W>
bool __write_mem(WriteKind, const bv<32>& addr, const bv<W>& data);

extern std::map<bv<32>, bv<32>> simulator_memory;

bool error(char const * msg);

void debug_mem();

class ContractException : std::exception 
{
};

#include "contract_runtime.tpp"
