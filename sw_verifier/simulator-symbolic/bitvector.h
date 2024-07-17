#pragma once
#include <stdint.h>
#include <unistd.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <memory>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <assert.h>
#include <iostream>
//#include <source_location>
#include <iomanip>

#include "PropVarSet.h"
#include "SymbolicExpression.h"

#ifdef SIMULATOR_BIT_LEVEL
#define bv bv_container
#else
#define bv bv_frag
#endif

#define info() (1 == 0) && std::cout
#define iprintf(...) fprintf (__VA_ARGS__)

#define bool_to_bits(b) (bv<1>(b))
#define zero_extend_template(b, new_size) ((b).template zero_extend_impl<new_size>())
#define zero_extend(b, new_size) ((b).zero_extend_impl<new_size>())
#define sign_extend(b, old_size, new_size) ([b]{static_assert((b).bitwidth() == (old_size));}, (b).sign_extend_impl<new_size>())
#define signed_lower_than(a, b, size) ([b]{static_assert((b).bitwidth() == (size));}, (a).signed_lower_than_impl((b)))
#define shift_right_arith(b, amm, size) ([b]{static_assert((b).bitwidth() == (size));}, (b).shift_right_arith_impl(amm))
#define bv_to_dec(b) ((b).to_dec().c_str())
#define bv_to_hex(b) ((b).to_hex().c_str())
#define bv_repr(b) ((b).to_string().c_str())
#define bv_cast_concrete(b) ((b).to_uint())
#define slice(b, end, start) ((b).slice_impl<end-start+1>(start))
#define bv_const(value, size) (bv<size>(value))

#define INVALID_OPCODE() do { printf("invalid opcode, ending simulation...\n"); throw ContractException(); } while (0)


template <size_t N>
class bv_frag
{
private:
    using value_t = unsigned _BitInt(N);

    //int TODO_delete[23];

    bool is_symbolic;
    std::shared_ptr<SymbolicExpression> symbolicValue;
    value_t value; // TODO use value_t

    size_t mysiz = N; // TODO remove

    template<size_t S>
    friend class bv_frag;

    template <size_t L> 
    friend std::ostream& operator<<(std::ostream&, const bv_frag<L>&);

    template <size_t L> 
    bv_frag(const bv_frag<L>& other)
        : value(other.value), is_symbolic(other.is_symbolic), symbolicValue(other.symbolicValue) {
    }

    inline bv_frag<N> linear_combination(const bv_frag<N>& other) const {
        if (is_symbolic && other.is_symbolic) {
            return bv_frag<N>(*symbolicValue ^ *other.symbolicValue);
        } else if (is_symbolic) {
            return bv_frag<N>(symbolicValue);
        } else if (other.is_symbolic) {
            return bv_frag<N>(other.symbolicValue);
        } else {
            assert(false);
        }
    }

    template <size_t L>
    inline bv_frag<N> nonlinear_combination(const SymbolicOperationType& type, const value_t identity_element, const bv_frag<L>& other) const {
        assert(is_symbolic || other.is_symbolic);
        std::vector<std::shared_ptr<SymbolicExpression>> vec;
        if (is_symbolic) {
            vec.push_back(symbolicValue);
        } else if (value == identity_element) {
            return bv_frag<N>(other);
        }
        if (other.is_symbolic) {
            vec.push_back(other.symbolicValue);
        } else if (other.value == identity_element) {
            return *this;
        }
        return bv_frag<N>(makeSymbolicOperation(type, vec));
    }

    template <size_t L>
    inline bv_frag<N> nonlinear_combination(const SymbolicOperationType& type, const value_t identity_element, const bv_frag<L>& other, const value_t overwrite_element) const {
        assert(is_symbolic || other.is_symbolic);
        std::vector<std::shared_ptr<SymbolicExpression>> vec;
        if (is_symbolic) {
            vec.push_back(symbolicValue);
        } else if (value == overwrite_element) {
            return *this;
        } else if (value == identity_element) {
            return bv_frag<N>(other);
        }
        if (other.is_symbolic) {
            vec.push_back(other.symbolicValue);
        } else if (other.value == overwrite_element) {
            return bv_frag<N>(other);
        } else if (other.value == identity_element) {
            assert(N == L);
            return *this;
        }
        return bv_frag<N>(makeSymbolicOperation(type, vec));
    }

    template <size_t L>
    inline bv_frag<N> nonlinear_shift(const SymbolicOperationType& type, const bv_frag<L>& other) const {
        assert(is_symbolic || other.is_symbolic);
        std::vector<std::shared_ptr<SymbolicExpression>> vec;
        constexpr unsigned _BitInt(N) identity_element_N = 0;
        constexpr unsigned _BitInt(L) identity_element_L = 0;
        if (is_symbolic) {
            vec.push_back(symbolicValue);
        } else if (value == identity_element_N) {
            return bv_frag<N>(identity_element_N);
        }
        if (other.is_symbolic) {
            vec.push_back(other.symbolicValue);
        } else if (other.value == identity_element_L) {
            return *this;
        }
        return bv_frag<N>(makeSymbolicOperation(type, vec));
    }

    inline bv_frag<N> unary_combination(const SymbolicOperationType& type) const {
        assert(is_symbolic);
        std::vector<std::shared_ptr<SymbolicExpression>> vec;
        vec.push_back(symbolicValue);
        return bv_frag<N>(makeSymbolicOperation(type, vec));
    }

public:
    constexpr size_t bitwidth() const {
        return N;
    }

    bool get_is_symbolic() const {
        return is_symbolic;
    }

    bv_frag() : value(0), is_symbolic(false), symbolicValue(nullptr) {}
    bv_frag(int v) : value(v), is_symbolic(false), symbolicValue(nullptr) {}
    bv_frag(const std::shared_ptr<SymbolicExpression>& _symbolicValue)
        : value(0), is_symbolic(true), symbolicValue(_symbolicValue) {
    }

    bv_frag(const bv_frag<N>& other)
        : value(other.value), is_symbolic(other.is_symbolic), symbolicValue(other.symbolicValue) {
    }

    std::shared_ptr<SymbolicExpression> get_combined_symbol() {
        return symbolicValue;
    }

    std::string to_dec() const
    {
        assert(!is_symbolic);
        return std::to_string((uint64_t)(value));
    }

    std::string to_hex() const
    {
        assert(!is_symbolic);
        return to_string();
    }

    std::string to_string() const
    {
        std::stringstream s;
        s << *this;
        return s.str();
    }

    unsigned int to_uint() const {
        assert(!is_symbolic);
        return value;
    }

    bv_frag<N> shift_right_arith_impl(unsigned long shift) const
    {
        if (is_symbolic) {
            return unary_combination(SymbolicOperationType::sym_shra);
        } else {
            return bv_frag<N>(((signed _BitInt(N))value) >> shift);
        }
    }

    template<size_t S>
    bv_frag<N> shift_right_arith_impl(const bv_frag<S>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(((signed _BitInt(N))value) >> other.value);
        } else {
            return nonlinear_shift(SymbolicOperationType::sym_shra, other);
        }
    }

    template<size_t S>
    bv_frag<S> sign_extend_impl() const
    {
        if (is_symbolic) {
            printf("symbolic sign_extend!\n");
            return bv_frag<S>(symbolicValue); // TODO needs bias?
        } else {
            return bv_frag<S>((signed _BitInt(S))(signed _BitInt(N))value);
        }
    }

    template<size_t S>
    bv_frag<S> zero_extend_impl() const
    {
        if (is_symbolic) {
            printf("symbolic zero_extend!\n");
            return bv_frag<S>(symbolicValue);
        } else {
            return bv_frag<S>(value);
        }
    }

    template<size_t S>
    bv_frag<S> slice_impl(const size_t start) const {
        assert(mysiz == bitwidth());
        if (is_symbolic) {
            printf("symbolic slice!\n");
            return bv_frag<S>(symbolicValue); // TODO needs bias?
        } else {
            return bv_frag<S>(value >> start);
        }
    }

    bv_frag<N>& operator =(const bv_frag<N>& other)
    {
        value = other.value;
        is_symbolic = other.is_symbolic;
        symbolicValue = other.symbolicValue;
        return *this;
    }

    template <size_t L>
    bv_frag<N> operator >>(const bv_frag<L>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(value >> other.value);
        } else {
            return nonlinear_shift(SymbolicOperationType::sym_shrl, other);
        }
    }

    bv_frag<N> operator >>(unsigned long other) const
    {
        if (is_symbolic) {
            return unary_combination(SymbolicOperationType::sym_shrl);
        } else {
            return bv_frag<N>(value >> other);
        }
    }

    bv_frag<N> operator +(int other) const
    {
        if (is_symbolic) {
            return unary_combination(SymbolicOperationType::sym_add);
        } else {
            return bv_frag<N>(value + other);
        }
    }

    bv_frag<N> operator -(const bv_frag<N>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(value - other.value);
        } else {
            return nonlinear_combination(SymbolicOperationType::sym_sub, 0, other);
        }
    }

    bv_frag<N> operator &(const bv_frag<N>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(value & other.value);
        } else {
            return nonlinear_combination(SymbolicOperationType::sym_and, -1, other, 0);
        }
    }

    bv_frag<N> operator |(const bv_frag<N>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(value | other.value);
        } else {
            return nonlinear_combination(SymbolicOperationType::sym_or, 0, other, -1);
        }
    }

    bv_frag<N> operator ^(const bv_frag<N>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(value ^ other.value);
        } else {
            return linear_combination(other);
        }
    }

    bv_frag<N> operator +(const bv_frag<N>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(value + other.value);
        } else {
            return nonlinear_combination(SymbolicOperationType::sym_add, 0, other);
        }
    }

    template <size_t L>
    bv_frag<N> operator <<(const bv_frag<L>& other) const
    {
        if (!is_symbolic && !other.is_symbolic) {
            return bv_frag<N>(value << other.value);
        } else {
            return nonlinear_shift(SymbolicOperationType::sym_shl, other);
        }
    }

    bv_frag<N> operator <<(const unsigned long& other) const
    {
        if (is_symbolic) {
            return unary_combination(SymbolicOperationType::sym_shl);
        } else {
            return bv_frag<N>(value << other);
        }
    }

    // base comparison operators

    bool operator ==(const bv_frag<N>& other) const
    {
        assert(!is_symbolic);
        //iprintf("compare %d: %d %d\n", N, value, other.value);
        return value == other.value;
    }

    bool operator <(const bv_frag<N>& other) const
    {
        assert(!is_symbolic);
        return value < other.value;
    }

    bool signed_lower_than_impl(const bv_frag<N>& other) const
    {
        assert(!is_symbolic);
        return ((signed _BitInt(N))value) < ((signed _BitInt(N))other.value);
    }

    // derived comparison operators
    bool operator >(const bv_frag<N>& other) const { return !(*this < other) && !(*this == other); }
    bool operator <=(const bv_frag<N>& other) const { return (*this < other) || (*this == other); }
    bool operator >=(const bv_frag<N>& other) const { return (*this > other) || (*this == other); }
    bool operator !=(const bv_frag<N>& other) const { return !(*this == other); }
};

enum bv_op {XOR, AND, OR};
enum bv_cmp_op {EQ, LT, SLT};

void* bv_fragment_resize(void* fragment, long source_size, long target_size, long offset);
void* bv_fragment_resize_signed(void* fragment, long source_size, long target_size, long offset);
void* bv_fragment_op(bv_op op, long size, void* a, void* b);
bool bv_fragment_cmp(bv_cmp_op op, long size, void* a, void* b);
void* bv_fragment_new(long size, long val);
void* bv_fragment_new(long size, std::shared_ptr<SymbolicExpression> val);
void bv_fragment_del(long size, void* a);
bool bv_fragment_symbolic(long size, void* a);

std::shared_ptr<SymbolicExpression> concat_symbols(const std::vector<std::shared_ptr<SymbolicExpression>> symbols);

template <size_t N>
class bv_container
{
private:
    typedef std::pair<size_t,void*> fragment_type; // TODO ensure freeing pointers
    std::vector<fragment_type> fragments;

    std::shared_ptr<SymbolicExpression> combined_symbol = nullptr;

    bv_container(std::vector<fragment_type>& fragments_)
        : fragments(fragments_) {
            //iprintf("bv_container<%d>(vector<fragments>)\n", N);
            constructor_check();
        }

    bv_container(const bv_frag<N>& frag_)
        : fragments({{N, new bv_frag<N>(frag_)}}) {
            //iprintf("bv_container<%d>(frag_)\n", N);
            constructor_check();
        }
    
    template<size_t S>
    friend class bv_container;

    bv_container<N> bitwise_operation(const bv_op op, const bv_container<N>& other) const {
        std::vector<fragment_type> new_fragments;
        size_t this_frag = 0;
        size_t other_frag = 0;
        size_t processed_bits = 0;
        size_t this_off = 0;
        size_t other_off = 0;
        while (processed_bits < N) {
            size_t this_size = this->fragments[this_frag].first;
            size_t other_size = other.fragments[other_frag].first;
            size_t fragsize = std::min(this_size, other_size);
            void* this_bv = bv_fragment_resize(this->fragments[this_frag].second, this_size, fragsize, this_off);
            void* other_bv = bv_fragment_resize(other.fragments[other_frag].second, other_size, fragsize, other_off);
            new_fragments.push_back({fragsize, bv_fragment_op(op, fragsize, this_bv, other_bv)});
            bv_fragment_del(fragsize, this_bv);
            bv_fragment_del(fragsize, other_bv);
            this_off += fragsize;
            other_off += fragsize;
            if (this_off == this_size) {
                this_frag += 1;
                this_off = 0;
            }
            if (other_off == other_size) {
                other_frag += 1;
                other_off = 0;
            }
            processed_bits += fragsize;
        }
        return bv_container<N>(new_fragments);
    }

public:
    
    constexpr size_t bitwidth() const {
        return N;
    }

    bv_container() : fragments({{N, new bv_frag<N>()}}) {
        //iprintf("bv_container<%d>()\n", N);
        constructor_check();
    }
    bv_container(int v) : fragments({{N, new bv_frag<N>(v)}}) {
        //iprintf("bv_container<%d>(%d)\n", N, v);
        constructor_check();
    }
    bv_container(const std::shared_ptr<SymbolicExpression>& _symbolicValue, const size_t size = N) {
        assert(size <= N);
        if (size == N) {
            fragments = {{N, new bv_frag<N>(_symbolicValue)}};
        } else {
            fragments = {{size, bv_fragment_new(size, _symbolicValue)}, {N-size, bv_fragment_new(N-size, 0)}};
        }
        //iprintf("bv_container<%d>(SYM)\n", N);
        constructor_check();
    }

    bv_container(const bv_container<N>& other) {
        assert(other.fragments.size() > 0);
        other.integrity_check();
        for (auto const& [size, frag] : other.fragments) {
            fragments.push_back({size, bv_fragment_resize(frag, size, size, 0)});
        }
        constructor_check();
    }

    void free_fragments() {
        for (auto const& [size, frag] : fragments) {
            bv_fragment_del(size, frag);
        }
        fragments.clear();
    }

    ~bv_container()
    {
        free_fragments();
    }

    void integrity_check() const {
        // TODO disable
        size_t sum = 0;
        for (auto const& [size, frag] : fragments) {
            sum += size;
        }
        if (sum != N) std::cout << sum << " vs " << N << std::endl;
        assert(sum == N);
    }

    void constructor_check() const {
        // TODO disable
        integrity_check();
        //iprintf("bv_container new @%p\n", this);
    }

    void memprint(char* name) {
        //iprintf("cont[%s] = *%p =", name, this);
        //for (int i = 0; i < 16; i++) iprintf("%02x ", *((uint8_t*)(this)+i)); iprintf("\n");
        //for (auto const& [size, frag] : fragments) {
        //    iprintf("frag[%d] *%p =", size, frag);
        //    for (int i = 0; i < 16; i++) iprintf("%02x ", *((uint8_t*)(frag)+i)); iprintf("\n");
        //}
    }

    std::shared_ptr<SymbolicExpression> get_combined_symbol() {
        if (combined_symbol != nullptr) {
            printf("symbol cache used\n");
            exit(123);
            return combined_symbol;
        }
        std::vector<std::shared_ptr<SymbolicExpression>> symbols;
        for (auto const& [size, frag] : fragments) {
            // TODO hack to avoid correct type cast
            auto sym = (*(bv_frag<N>*)frag).get_combined_symbol();
            if (sym != nullptr)
                symbols.push_back(sym);
        }
        integrity_check();
        return (combined_symbol = concat_symbols(symbols));
    }

    std::string to_dec() const
    {
        if (fragments.size() == 1) {
            return ((bv_frag<N>*)fragments[0].second)->to_dec();
        }
        integrity_check();
        return "TODO";
    }

    std::string to_hex() const
    {
        if (fragments.size() == 1) {
            return ((bv_frag<N>*)fragments[0].second)->to_hex();
        }
        integrity_check();
        return "TODO";
    }

    std::string to_string() const
    {
        if (fragments.size() == 1) {
            return ((bv_frag<N>*)fragments[0].second)->to_string();
        }
        std::string res = "{";
        size_t offset = 0;
        for (auto const& [size, frag] : fragments) {
            res += fmt::format("[{}:{}]", offset+size-1, offset);
            res += ((bv_frag<N>*)frag)->to_string(); // TODO type safe
            res += ", ";
            offset += size;
        }
        res += "}";

        return res;
    }

    unsigned int to_uint() const {
        assert(fragments.size() == 1);
        return ((bv_frag<N>*)fragments[0].second)->to_uint();
    }

    bv_container<N> shift_right_arith_impl(unsigned long shift) const
    {
        assert(fragments.size() == 1);
        return bv_container<N>(((bv_frag<N>*)fragments[0].second)->shift_right_arith_impl(shift));
    }

    template<size_t S>
    bv_container<N> shift_right_arith_impl(const bv_container<S>& other) const
    {
        assert((fragments.size() == 1) && (other.fragments.size() == 1));
        return bv_container<N>(((bv_frag<N>*)fragments[0].second)->shift_right_arith_impl(*((bv_frag<S>*)other.fragments[0].second)));
    }

    template<size_t S>
    bv_container<S> sign_extend_impl() const
    {
        static_assert(S >= N);
        //assert(fragments.size() == 1);
        //return bv_container<S>(((bv_frag<N>*)fragments[0].second)->template sign_extend_impl<S>());
        std::vector<fragment_type> new_fragments;
        bool done = false;
        long idx = 0;
        for (auto const& [size, frag] : fragments) {
            if (idx == fragments.size() - 1) {
                const auto newsize = size+(S-N);
                new_fragments.push_back({newsize, bv_fragment_resize_signed(frag, size, newsize, 0)});
            } else {
                new_fragments.push_back({size, bv_fragment_resize(frag, size, size, 0)});
            }
        }
        return bv_container<S>(new_fragments);
    }

    template<size_t S>
    bv_container<S> zero_extend_impl() const
    {
        static_assert(S >= N);
        if constexpr (S == N) {
            return *this;
        } else {
            std::vector<fragment_type> new_fragments;
            bool done = false;
            long idx = 0;
            for (auto const& [size, frag] : fragments) {
                if ((idx == fragments.size() - 1) && !bv_fragment_symbolic(size, frag)) {
                    const auto newsize = size+(S-N);
                    new_fragments.push_back({newsize, bv_fragment_resize(frag, size, newsize, 0)});
                    done = true;
                } else {
                    new_fragments.push_back({size, bv_fragment_resize(frag, size, size, 0)});
                }
            }
            if (!done)
                new_fragments.push_back({S-N, new bv_frag<S-N>(0)});
            return bv_container<S>(new_fragments);
        }
    }

    template<size_t S>
    bv_container<S> slice_impl(const size_t start) const {
        info() << "bv_container::slice_impl N=" << N << " S=" << S << " start=" << start << " this=" << this << std::endl;
        static_assert(S <= N);
        assert(S+start <= N);
        std::vector<fragment_type> new_fragments;
        size_t offset = 0;
        //info() << "slice_impl N=" << N << " S=" << S << "start=" << std::endl;
        for (auto const& [size, frag] : fragments) {
            //info() << "slice size " << size << std::endl;
            if (offset+size <= start) continue;
            if (S+start <= offset) break;
            const size_t start_offset = std::max((ssize_t)start-(ssize_t)offset, 0l);
            const size_t end_offset = std::min((ssize_t)S+(ssize_t)start-(ssize_t)offset, (ssize_t)size);
            const size_t new_size = end_offset-start_offset;
            new_fragments.push_back({new_size, bv_fragment_resize(frag, size, new_size, start_offset)});
            info() << "slice size " << size << " -> " << end_offset << ":" << start_offset << std::endl;
            offset += size;
        }
        integrity_check();

        info() << "result:" << std::endl;
        for (auto const& [size, frag] : new_fragments) {
            info() << "slice size " << size << std::endl;
        }
        integrity_check();
        return bv_container<S>(new_fragments);
    }

    bv_container<N>& operator =(const bv_container<N>& other)
    {
        //iprintf("bv_container<%d>& operator = (this=%p, other=%p, other.fragments.size = %d)\n", N, this, &other, other.fragments.size());
        integrity_check();
        free_fragments();
        assert(other.fragments.size() > 0);
        other.integrity_check();
        for (auto const& [size, frag] : other.fragments) {
            fragments.push_back({size, bv_fragment_resize(frag, size, size, 0)});
        }
        integrity_check();
        combined_symbol = other.combined_symbol; // TODO correct?
        return *this;
    }

    template <size_t L>
    bv_container<N> operator >>(const bv_container<L>& other) const
    {
        assert((fragments.size() == 1) && (other.fragments.size() == 1));
        return bv_container<N>(*((bv_frag<N>*)fragments[0].second) >> *((bv_frag<L>*)other.fragments[0].second));
    }

    bv_container<N> operator >>(unsigned long other) const
    {
        assert(fragments.size() == 1);
        return bv_container<N>(*((bv_frag<N>*)fragments[0].second) >> other);
    }

    bv_container<N> operator +(int other) const
    {
        assert(fragments.size() == 1);
        return bv_container<N>(*((bv_frag<N>*)fragments[0].second) + other);
    }

    bv_container<N> operator -(const bv_container<N>& other) const
    {
        assert((fragments.size() == 1) && (other.fragments.size() == 1));
        return bv_container<N>(*((bv_frag<N>*)fragments[0].second) - *((bv_frag<N>*)other.fragments[0].second));
    }

    bv_container<N> operator &(const bv_container<N>& other) const
    {
        return bitwise_operation(bv_op::AND, other);
    }

    bv_container<N> operator |(const bv_container<N>& other) const
    {
        return bitwise_operation(bv_op::OR, other);
    }

    bv_container<N> operator ^(const bv_container<N>& other) const
    {
        return bitwise_operation(bv_op::XOR, other);
    }

    bv_container<N> operator +(const bv_container<N>& other) const
    {
        assert((fragments.size() == 1) && (other.fragments.size() == 1));
        return bv_container<N>(*((bv_frag<N>*)fragments[0].second) + *((bv_frag<N>*)other.fragments[0].second));
    }

    template <size_t L>
    bv_container<N> operator <<(const bv_container<L>& other) const
    {
        if ((fragments.size() == 1) && (other.fragments.size() == 1))
            return bv_container<N>(*((bv_frag<N>*)fragments[0].second) << *((bv_frag<L>*)other.fragments[0].second));
        const size_t offset = other.to_uint();
        return *this << offset;
    }

    bv_container<N> operator <<(const unsigned long& offset) const
    {
        assert(offset < N);
        if (fragments.size() == 1)
            return bv_container<N>(*((bv_frag<N>*)fragments[0].second) << offset);
        if (offset == 0) {
            return *this;
        }
        size_t bits_processed = 0;
        std::vector<fragment_type> new_fragments;
        new_fragments.push_back({offset, bv_fragment_new(offset, 0)});
        for (auto const& [size, frag] : fragments) {
            if (bits_processed >= N-offset) {
                break;
            }
            const size_t new_size = std::min((ssize_t)size, (ssize_t)N-(ssize_t)offset-(ssize_t)bits_processed);
            new_fragments.push_back({new_size, bv_fragment_resize(frag, size, new_size, 0)});
            info() << "slice size " << new_size << std::endl;
            bits_processed += size;
        }
        info() << "slice size " << offset << std::endl;
        return bv_container<N>(new_fragments);
    }

    // base comparison operators

    bool operator ==(const bv_container<N>& other) const
    {
        integrity_check();
        size_t this_frag = 0;
        size_t other_frag = 0;
        size_t processed_bits = 0;
        while (processed_bits < N) {
            size_t this_size = this->fragments[this_frag].first;
            size_t other_size = other.fragments[other_frag].first;
            //if (this_size != other_size) info() << "not equal len" << std::endl;
            if (this_size != other_size) return false;
            void* this_bv = this->fragments[this_frag].second;
            void* other_bv = other.fragments[other_frag].second;
            //if (!bv_fragment_cmp(bv_cmp_op::EQ, this_size, this_bv, other_bv)) info() << "not equal value" << std::endl;
            if (!bv_fragment_cmp(bv_cmp_op::EQ, this_size, this_bv, other_bv)) return false;
            processed_bits += this_size;
        }
        return true;
    }

    bool operator <(const bv_container<N>& other) const
    {
        assert((fragments.size() == 1) && (other.fragments.size() == 1));
        return *((bv_frag<N>*)fragments[0].second) < *((bv_frag<N>*)other.fragments[0].second);
    }

    bool signed_lower_than_impl(const bv_container<N>& other) const
    {
        assert((fragments.size() == 1) && (other.fragments.size() == 1));
        return ((bv_frag<N>*)fragments[0].second)->signed_lower_than_impl(*((bv_frag<N>*)other.fragments[0].second));
    }

    // derived comparison operators
    bool operator >(const bv_container<N>& other) const { return !(*this < other) && !(*this == other); }
    bool operator <=(const bv_container<N>& other) const { return (*this < other) || (*this == other); }
    bool operator >=(const bv_container<N>& other) const { return (*this > other) || (*this == other); }
    bool operator !=(const bv_container<N>& other) const { return !(*this == other); }
};

template <size_t N>
std::ostream& operator<<(std::ostream &strm, const bv_frag<N> &b) {
    static_assert(N <= 64, "printing large bitvectors not implemented");
    if (b.is_symbolic) {
        std::stringstream s;
        s << *b.symbolicValue;
        const std::string st = s.str();
        return strm << fmt::format("{:<{}}", st, (int)((N-1)/4+3));
    } else {
        return strm << fmt::format("0x{:0{}x}", (uint64_t)b.value, (int)((N-1)/4+1));
    }
}

template <size_t N>
std::ostream& operator<<(std::ostream &strm, const bv_container<N> &b) {
    static_assert(N <= 64, "printing large bitvectors not implemented");
    assert(false);
}


//template<size_t N>
//bv<N> accumulate_leaks(bv<N> v) {
//    return v;
//}
//template<size_t N, size_t... Ns>
//bv<N> accumulate_leaks(std::vector<std::shared_ptr<SymbolicExpression>>& vec, bv<N> first, bv<Ns>... args) {
//    vec.push_back(first.get_combined_symbol());
//    return first + accumulate_leaks(args...);
//}

template<size_t N>
void accumulate_leaks(std::vector<std::shared_ptr<SymbolicExpression>>& vec, bv<N> first) {
    auto sym = first.get_combined_symbol();
    //if (sym)
    vec.push_back(sym);
}

template<size_t N, size_t... Ns>
void accumulate_leaks(std::vector<std::shared_ptr<SymbolicExpression>>& vec, bv<N> first, bv<Ns>... args) {
    //static_assert(sizeof(...args) > 0);
    //if (sizeof...(args) == 0) {
    //  return first;
    //}
    auto sym = first.get_combined_symbol();
    //if (sym)
    vec.push_back(sym);
    accumulate_leaks(vec, args...);
    //return first + accumulate_leaks(args...);
}

void leak_vector(const std::string& contract_location, bv<32> pc, const std::vector<std::shared_ptr<SymbolicExpression>>& vec);

template<size_t... Ns>
void leak_impl(const std::string& file_path, const std::string& func, int line, bv<32> pc, bv<Ns>... args) {
    std::vector<std::shared_ptr<SymbolicExpression>> vec;
    accumulate_leaks(vec, args...);
    if (vec.size() == 0) {
        return;
    }
    std::string file = file_path.substr(file_path.find_last_of('/')+1);
    std::string contract_location = file + ":" + std::to_string(line) + " " + func;
    leak_vector(contract_location, pc, vec);
}

#define leak(args...) leak_impl(__FILE__, __func__, __LINE__, INSTRUCTION_POINTER, args)

// TODO
//template <size_t L1, size_t L2> 
//bv<N>& concat(const bv<L1>& b1, const bv<L2>& b2)
//{
//}



template <size_t N>
struct fmt::formatter<bv<N>> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    auto format(const bv<N>& val, format_context& ctx) {
        return ctx.out() << val;
    }
};
