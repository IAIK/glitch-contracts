#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <memory>
#include <iostream>

#include "Options.h"

#include "../contract/contract.h"
#include "contract_runtime.h"
#include "SymbolicManager.h"
#include "bitvector.h"

#include <backward.hpp>
namespace backward {
backward::SignalHandling sh;
} // namespace backward

class SymbolicManager;

struct slice {
    uint64_t end;
    uint64_t start;
};
struct position {
    std::string id;
    std::vector<slice> indices;
};

struct LabelException : public std::exception
{
   std::string s;
   LabelException(std::string ss) : s(ss) {}
   ~LabelException() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};

uint64_t token_to_int(const std::string& token) {
    try {
        if (token.size() > 2 && token[0] == '0' && token[1] == 'x') {
            return std::stoul(token, nullptr, 16);
        } else {
            return std::stoul(token);
        }
    } catch (std::invalid_argument e) {
        throw LabelException("could not parse token '" + token + "' as number");
    }
}

position parse_indices(const std::string& in) {
    position res;
    std::string s = strip(in);
    if (s.size() == 0)
        return res;
    while (s[s.size()-1] == ']')
    {
        size_t pos = s.rfind("[");
        assert(pos != std::string::npos);
        std::string index = s.substr(pos + 1, s.size()-1);
        s = s.substr(0, pos);
        s = strip(s);

        pos = index.rfind(":");
        if(pos == std::string::npos)
        {
            uint64_t i = token_to_int(index);
            res.indices.push_back({i, i});
        }
        else
        {
            std::string end = index.substr(pos + 1);
            std::string start = index.substr(0, pos);
            res.indices.push_back({token_to_int(start), token_to_int(end)});
        }
    }
    res.id = strip(s);
    std::reverse(res.indices.begin(), res.indices.end());
    return res;
}

void parse_labels(const char* label_path, const std::shared_ptr<SymbolicManager>& mgr)
{
    std::ifstream f;
    f.open(label_path);
    if (!f) {
        throw std::ifstream::failure("file does not exist");
    }
    std::string line;
    while(true)
    {
        std::getline(f, line);
        std::cout << "line: " << line << std::endl;
        if(f.eof()) break;
        if(line.empty() || line[0] == '#') continue;
        size_t pos = line.rfind("=");
        assert(pos != std::string::npos);
        std::string target_s = line.substr(0, pos);
        std::string label_s = line.substr(pos + 1);

        // label parsing
        position label = parse_indices(label_s);
        LabelType lab_type = label_type(label.id);

        // target parsing
        position target = parse_indices(target_s);
        std::string target_type_s = target.id;
        std::string id_raw;
        pos = target_type_s.find(".");
        if(pos != std::string::npos) {
            id_raw = target_type_s.substr(pos + 1);
            target_type_s = target_type_s.substr(0, pos);
        }

        // check target size
        size_t target_size = 32; // TODO define
        if (target_type_s == "reg") {
            if (target.indices.size() >= 1) {
                target_size = target.indices[0].end - target.indices[0].start + 1;
            }
        } else if (target_type_s == "mem") {
            if (target.indices.size() >= 2) {
                target_size = target.indices[1].end - target.indices[1].start + 1;
            }
        }

        // process label
        // TODO check for overlaps in secrets and/or merge secret regions
        bv<32> value;
        if (lab_type == LabelType::LABEL_CONSTANT) {
            value = token_to_int(label.id);
        } else if (lab_type == LabelType::LABEL_SECRET) {
            assert(label.indices.size() >= 1);
            assert((label.indices[0].end - label.indices[0].start + 1) == target_size);
            value = mgr->make_secret_symbol<32>(label.indices[0].start, label.indices[0].end);
        } else if (lab_type == LabelType::LABEL_MASK) { // TODO handle sub-word sizes
            value = mgr->make_mask_symbol<32>(target_size);
        } else if (lab_type == LabelType::LABEL_RNG) {
            std::cout << "rng address: 0x" << std::hex << target.indices[0].start << std::dec << std::endl;
            assert(target_type_s == "mem");
            mmio_rng_addr = target.indices[0].start; // TODO checks
            mmio_rng_enabled = true;
        } else {
            throw std::logic_error("Function not yet implemented");
        }

        std::cout << "new value: " << bv_repr(value) << std::endl;

        // process target
        if (target_type_s == "reg") {
            if (target.indices.size() >= 1) {
                assert(target.indices[0].start == 0); // sub-word assignment not implemented
            }
            set_register_word(id_raw.c_str(), value);
        } else if (target_type_s == "mem") {
            assert(target.indices.size() >= 1);
            if (target.indices.size() >= 2) {
#ifdef SIMULATOR_BIT_LEVEL
                value = value << target.indices[1].start;
                std::cout << "new new value: " << bv_repr(value) << std::endl;
#else
                assert(target.indices[1].start == 0); // sub-word assignment not implemented
#endif
            }
            assert(target.indices[0].start == target.indices[0].end); // TODO multiple words in one line
            for (uint64_t i = target.indices[0].start; i <= target.indices[0].end; i++) { // TODO restructure this
                bv<32> addr = i;
                __write_mem_bits<32>(data_write, addr, value);
            }
        } else {
            std::cout << "???" << std::endl;
            throw std::ifstream::failure("bad");
        }
    }
    f.close();
}

void load_program(const char* program_path)
{
    std::ifstream f;
    f.open(program_path);
    if (!f) {
        throw std::ifstream::failure("file does not exist");
    }
    std::string line;
    while(true)
    {
        std::getline(f, line);
        std::cout << "line: " << line << std::endl;
        if(f.eof()) break;
        if(line.empty() || line[0] == '#') continue;
        size_t pos = line.find(":");
        assert(pos != std::string::npos);
        std::string addr = strip(line.substr(0, pos));
        std::string value = strip(line.substr(pos + 2));
        int share_num = -1;
        size_t num_pos = value.find(' ');
        assert(num_pos != std::string::npos);
        value = value.substr(0, num_pos);
        uint64_t addr_i = std::stoul(addr, nullptr, 16);
        uint64_t value_i = std::stoul(value, nullptr, 16);
        write_mem(addr_i, 4, value_i);
    }
    f.close();
}

int main(int argc, const char* argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0); // TODO remove
    std::cout << "simulator start"  << std::endl;
    int ret_val = 0;
    Options* opts = nullptr;
    std::shared_ptr<SymbolicManager> symbolicManager;

#ifdef DEBUG_SOLVER
    std::ofstream d("debug.cnf");
#endif

    contract_init();

    try { opts = new Options(argc, argv); }
    catch (const OptionsException& e)
    {
        if (std::string("") != e.what())
        {
            std::cout << "Error while parsing options: " << e.what() << std::endl;
            ret_val = 1; goto cleanup;
        }
        return 0;
    }
    std::cout << "parsing program..." << std::endl;
    symbolicManager = std::make_shared<SymbolicManager>(opts->order);
    globalSymbolicManager = symbolicManager;

#ifdef DEBUG_SOLVER
    symbolicManager->set_debug_file(d);
#endif

    try { load_program(opts->program_path.c_str()); }
    catch (const std::ifstream::failure& e)
    {
        std::cout << "Failed reading program: " << e.what() << std::endl;
        ret_val = 2; goto cleanup;
    }
    std::cout << "parsing labels..." << std::endl;
    for (const auto& path : opts->label_paths) {
        try { parse_labels(path.c_str(), symbolicManager); }
        catch (const std::ifstream::failure& e)
        {
            std::cout << "Failed reading labels: " << e.what() << std::endl;
            ret_val = 3; goto cleanup;
        } 
    }
    std::cout << "params:" << std::endl;
    std::cout << *opts;

    std::cout << "simulation start" << std::endl;

    try { contract_simulate(); }
    catch (const ContractException& e)
    {
        std::cout << "ContractException caught" << std::endl;
    }
    std::cout << "checking leaks" << std::endl;
    if (!symbolicManager->check_leaks()) {
        std::cout << "leak checks failed" << std::endl;
        ret_val = 4; goto cleanup;
    }
    std::cout << "checking done" << std::endl;

cleanup:
    delete opts;
    return ret_val;
}
