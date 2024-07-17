#include "common.h"
#include <cassert>
#include <iostream>

LabelType label_type(const std::string& x)
{
    if(x == "secret") return LabelType::LABEL_SECRET;
    if(x == "mask") return LabelType::LABEL_MASK;
    if(x == "rng") return LabelType::LABEL_RNG;
    if(x == "unimportant") return LabelType::LABEL_OTHER;
    return LabelType::LABEL_CONSTANT;
}

std::string strip(const std::string &inpt)
{
    auto start_it = inpt.begin();
    auto end_it = inpt.rbegin();
    while (std::isspace(*start_it))
        ++start_it;
    while (std::isspace(*end_it))
        ++end_it;
    return std::string(start_it, end_it.base());
}

std::ostream& operator<<(std::ostream& strm, const var_t& var)
{
    if (var == ZERO) {
        strm << "ZERO";
    } else if (var == ONE) {
        strm << "ONE";
    } else {
        strm << (int32_t)var;
    }
    return strm;
}
