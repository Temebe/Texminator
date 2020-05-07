#include "../include/Environment.h"

void Environment::createNewScope(const ScopeType type) {

}

std::optional<Value> Environment::getVariable(const std::string &name_) {
    auto currentScope = scopes.begin();
    std::optional<Value> result;

    while (currentScope != scopes.end()) {
        result = currentScope->getVariable(name_);
        if ((currentScope++)->getType() == ScopeType::function || result) {
            break;
        }
    }

    if (result) {
        return result;
    }

    return globalScope.getVariable(name_);
}
