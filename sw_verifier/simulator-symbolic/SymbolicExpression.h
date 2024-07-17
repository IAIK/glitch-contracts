#pragma once
#include <inttypes.h>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include <cassert>
#include "PropVarSet.h"

class SymbolicManager;

enum SymbolicOperationType {
    sym_add,
    sym_sub,
    sym_shl,
    sym_shra,
    sym_shrl,
    sym_xor,
    sym_and,
    sym_or,
    sym_concat,
    sym_leak
};

std::string to_string(SymbolicOperationType type);

class Symbol;
class SymbolicOperation;

class SymbolicExpression
    : public std::enable_shared_from_this<SymbolicExpression>
{
protected:
    std::shared_ptr<SymbolicManager> manager;
    std::unique_ptr<PropVarSet> propVarSet;
public:
    std::shared_ptr<SymbolicManager> get_manager() { return manager; }
    SymbolicExpression(std::shared_ptr<SymbolicManager> _manager, std::unique_ptr<PropVarSet> _propVarSet)
        : manager(_manager), propVarSet(std::move(_propVarSet)) { } // assert(propVarSet != nullptr); };
    virtual void write(std::ostream &strm) const = 0;
    virtual void constructPropVarSet(SatSolver* solver) = 0;
    virtual bool isOp() const = 0;
    virtual ~SymbolicExpression() = default;

    const PropVarSet& getPropVarSet() { return *propVarSet; }

    std::shared_ptr<SymbolicExpression> operator ^(SymbolicExpression& other);
    friend std::shared_ptr<SymbolicOperation> makeSymbolicOperation(SymbolicOperationType type, std::vector<std::shared_ptr<SymbolicExpression>> children);
    friend std::unique_ptr<PropVarSet> makeNonlinearCombination(const std::vector<std::shared_ptr<SymbolicExpression>>& children, std::vector<var_t>& bias_vars);
    friend class SymbolicOperation;
    friend class SymbolicManager;
};

class Symbol : public SymbolicExpression {
protected:
    size_t id;
    std::string name;
public:
    Symbol(std::shared_ptr<SymbolicManager> _manager, std::unique_ptr<PropVarSet> _propVarSet, size_t id, std::string _name);
    Symbol() = delete;
    virtual void write(std::ostream &strm) const;
    virtual void constructPropVarSet(SatSolver* solver);
    virtual bool isOp() const { return false; }
    size_t getId() { return id; }
    const std::string& getName() { return name; }
    virtual ~Symbol() = default;
};

class SymbolicOperation : public SymbolicExpression {
protected:
    SymbolicOperationType type;
    std::vector<std::shared_ptr<SymbolicExpression>> children;
public:
    SymbolicOperation(std::shared_ptr<SymbolicManager> _manager, std::unique_ptr<PropVarSet> _propVarSet, 
                      SymbolicOperationType _type, std::vector<std::shared_ptr<SymbolicExpression>> _children)
        : SymbolicExpression(_manager, std::move(_propVarSet)), type(_type), children(_children) {};
    SymbolicOperation() = delete;
    virtual void write(std::ostream &strm) const;
    virtual void constructPropVarSet(SatSolver* solver);
    virtual bool isOp() const { return true; }
    virtual ~SymbolicOperation() = default;
    friend void extract_linear(std::set<SymbolicExpression*>& set, std::shared_ptr<SymbolicExpression> sym); // TODO remove?
    friend class SymbolicManager;
};

class SymbolicLeak : public SymbolicOperation {
protected:
    uint32_t contract_location;
    uint32_t pc;
public:
    SymbolicLeak(std::shared_ptr<SymbolicManager> _manager, std::unique_ptr<PropVarSet> _propVarSet, 
                 std::vector<std::shared_ptr<SymbolicExpression>> _children)
        : SymbolicOperation(_manager, std::move(_propVarSet), SymbolicOperationType::sym_leak, _children) {};
    SymbolicLeak() = delete;
    void set_contract_location(uint32_t v) { contract_location = v; }
    void set_pc(uint32_t v) { pc = v; }
    virtual void write(std::ostream &strm) const;
    virtual ~SymbolicLeak() = default;
};

std::ostream& operator<<(std::ostream &strm, const SymbolicExpression& sym);
