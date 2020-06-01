#ifndef TEXMINATOR_ENVIRONMENT_H
#define TEXMINATOR_ENVIRONMENT_H

#include <deque>
#include <memory>

#include "Scope.h"

class Environment {
public:
    void createNewScope(ScopeType type);
    void destroyCurrentScope();
    void addVariable(const std::string &name_, const Value& variable_);
    void addFunction(const std::string &name_, Function& function_);
    void addGlobalVariable(const std::string &name_, const Value& variable_);
    void addGlobalFunction(const std::string &name_, Function &function_);
    std::optional<Value> getVariable(const std::string &name_);
    Function& getFunction(const std::string &name_, const std::list<Parameter> &parameters_);
    Function& getFunction(const std::string &name_, const std::list<Value> &parameters_);
    void setVariable(const std::string &name_, const Value& variable_);
    void setReturnValue(const Value& value_);
    Value getReturnValue();

private:
    std::deque<Scope> scopes;
    // global scope is reserved for special functions and variables
    // therefore there is no option to add nothing to it by accident - you have to use special methods
    Scope globalScope = Scope(local);
    static const std::string returnValueName;

};


#endif //TEXMINATOR_ENVIRONMENT_H
