#ifndef TEXMINATOR_ENVIRONMENT_H
#define TEXMINATOR_ENVIRONMENT_H

#include <deque>
#include <memory>

#include "Scope.h"

class Environment {
public:
    void createNewScope(const ScopeType type);
    std::optional<Value> getVariable(const std::string &name_);

private:
    std::deque<Scope> scopes;
    Scope globalScope;

};


#endif //TEXMINATOR_ENVIRONMENT_H
