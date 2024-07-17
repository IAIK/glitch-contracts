#pragma once
#include "common.h"
#include <string>
#include <stdexcept>
#include <vector>


#define RST_NAME "rst_i"

struct Options {
    std::vector<std::string> label_paths;
    std::string program_path;
    size_t order;
    VerifMode mode;
    size_t num_leaks;
    Options(int argc, const char* argv[]);
    friend std::ostream& operator<<(std::ostream& out, Options& opts);
};

struct OptionsException : std::logic_error
{
    OptionsException(const char* msg) : std::logic_error(msg) {};
};
