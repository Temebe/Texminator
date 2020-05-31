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
    std::optional<Value> getVariable(const std::string &name_);
    const Function& getFunction(const std::string &name_, const std::list<Parameter> &parameters_);
    void setVariable(const std::string &name_, const Value& variable_);
    void setReturnValue(const Value& value_);
    Value getReturnValue();

private:
    std::deque<Scope> scopes;
    static const std::string returnValueName;

};


#endif //TEXMINATOR_ENVIRONMENT_H
