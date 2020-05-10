#ifndef TEXMINATOR_ENVIRONMENT_H
#define TEXMINATOR_ENVIRONMENT_H

#include <deque>
#include <memory>

#include "Scope.h"

class Environment {
public:
    void createNewScope(ScopeType type);
    void destroyCurrentScope();
    std::optional<Value> getVariable(const std::string &name_);
    const Function& getFunction(const std::string &name_, const std::list<Parameter> &parameters_);

private:
    std::deque<Scope> scopes;

};


#endif //TEXMINATOR_ENVIRONMENT_H
