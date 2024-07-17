#pragma once
#include <string>
#include <map>
#include <cctype>
#include <iostream>

/// debugging settings
#define VERBOSITY 10L
#define DEBUG(l) (l <= VERBOSITY) && std::cout

/// Type for solver variables and literals
//using var_t = int32_t;
enum var_t : int32_t {};
/// Type used for label indexing
using lidx_t = uint32_t;

inline var_t operator -(const var_t& val) {
    return var_t(-int32_t(val));
}

std::ostream& operator<<(std::ostream& strm, const var_t& var);

/// There are only two illegal int values for solver literals.
/// These are 0 and INT_MIN, whose negation is themselves.
inline bool is_legal(var_t x) { return (-x != x); }

/// We sacrifice one variable to use as true(ONE) and false(ZERO).
/// Most solvers do not support so many variables anyway.
constexpr var_t ZERO = (var_t)-INT32_MAX;
constexpr var_t ONE  = (var_t)+INT32_MAX;

enum class VerifMode : char {MODE_STABLE, MODE_TRANS};
enum class LabelType : char {LABEL_SECRET, LABEL_MASK, LABEL_RNG, LABEL_OTHER, LABEL_CONSTANT};

LabelType label_type(const std::string& x);

std::string strip(const std::string &inpt);
