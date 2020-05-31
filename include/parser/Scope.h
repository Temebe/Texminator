#ifndef TEXMINATOR_SCOPE_H
#define TEXMINATOR_SCOPE_H

#include <unordered_map>

#include "Value.h"
#include "Function.h"

using FunctionMap = std::unordered_multimap<std::string, Function>;

enum ScopeType {
        local, function
};

// TODO Currently
class Scope {
public:
    explicit Scope(ScopeType type_);
    ScopeType getType() const;
    bool containsVariable(const std::string &name_) const;
    bool containsFunction(const std::string &name_, const std::list<Parameter> &parameters_) const;
    const Function& getFunction(const std::string &name_, const std::list<Parameter> &parameters_) const;
    std::optional<Value> getVariable(const std::string &name_) const;
    void addVariable(const std::string &name_, const Value& variable_);
    void replaceVariable(const std::string &name_, const Value& variable_);
    void addFunction(const std::string &name_, Function& function_);

private:
    FunctionMap::const_iterator findFunction(const std::string &name_, const std::list<Parameter> &parameters_) const;

    std::unordered_map<std::string, Value> variables;
    FunctionMap functions;
    ScopeType type;

};


#endif //TEXMINATOR_SCOPE_H
