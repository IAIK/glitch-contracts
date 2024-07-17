#include "SatSolver.h"
#include <cassert>
#include <iostream>

#include "PropVarSet.h"

var_t SatSolver::make_xor(var_t a, var_t b)
{
#ifdef OPT_SAT_ENCODING_CACHE
    assert(a != ONE && a != ZERO);
    assert(b != ONE && b != ZERO);
    bool invert = ((a < 0) ^ (b < 0));
    var_t& cached = linear_cache[{std::abs(a), std::abs(b)}];
    if (cached != 0) {
        return invert ? -cached : cached;
    }
#endif
    var_t c = new_var();
#ifdef DEBUG_SOLVER
        debug_file << "# " << c << " = " << a << " ^ " << b << std::endl;
#endif
    add_clause(-a, -b, -c);
    add_clause(+a, +b, -c);
    add_clause(-a, +b, +c);
    add_clause(+a, -b, +c);
#ifdef OPT_SAT_ENCODING_CACHE
    cached = invert ? -c : c;
#endif
    return c;
}

var_t SatSolver::make_and(var_t a, var_t b)
{
    assert(a != ONE && a != ZERO);
    assert(b != ONE && b != ZERO);
#ifdef OPT_SAT_ENCODING_CACHE
    var_t cached& = nonlinear_cache[{a, b}];
    if (cached != 0) {
        return cached;
    }
#endif
    var_t c = new_var();
#ifdef DEBUG_SOLVER
        debug_file << "# " << c << " = " << a << " & " << b << std::endl;
#endif
    add_clause(+a, -c);
    add_clause(+b, -c);
    add_clause(-a, -b, +c);
#ifdef OPT_SAT_ENCODING_CACHE
    cached = c;
#endif
    return c;
}

// implementation of https://link.springer.com/content/pdf/10.1007%2F11564751_73.pdf
void SatSolver::make_atmost_k_of_n(size_t k, const std::vector<var_t>& xs)
{
    assert(k > 0);
    const auto n = xs.size();
    if (k >= n)
        return;
    std::vector<var_t> ss;
    ss.reserve(n);
    for (uint32_t i = 0; i < n ; i++)
        ss.push_back(new_vars(k));

#ifdef DEBUG_SOLVER
    debug_file << "# " << ss[0] << ".." << ss[0]+(n*k)-1 ;
    debug_file << " = " << k << " of ";
    for (uint32_t i = 0; i < n; i++) {
        debug_file << xs[i] << ' ';
    }
    debug_file << std::endl;
#endif

    add_clause(-xs[0], ss[0]);
    for (int j = 1; j < k; j++)
        add_clause(-(ss[0] + j));
    for (int i = 1; i < n-1; i++)
    {
        add_clause(-xs[i], ss[i]);
        add_clause(-ss[i-1], ss[i]);
        for(int j = 1; j < k; j++)
        {
            add_clause(-xs[i], -(ss[i-1] + j-1), ss[i] + j);
            add_clause(-(ss[i-1] + j), ss[i] + j);
        }
        add_clause(-xs[i], -(ss[i-1] + k - 1));
    }
    add_clause(-xs[n-1], -(ss[n-2] + k - 1));
}

var_t SatSolver::make_implies_all(std::vector<var_t>& list)
{
    assert(!list.empty());
    var_t p = new_var();

#ifdef DEBUG_SOLVER
    debug_file << "# " << p << " implies ";
    for (const auto& x : list) {
        debug_file << x << ' ';
    }
    debug_file << std::endl;
#endif

    for(var_t x : list) {
        add_clause(x, -p);
    }
    return p;
}
