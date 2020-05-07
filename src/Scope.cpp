#include "../include/Scope.h"

ScopeType Scope::getType() {
    return type;
}

std::optional<Value> Scope::getVariable(const std::string &name_) {
    auto variableIterator = variables.find(name_);
    if (variableIterator == variables.end()) {
        return std::optional<Value>();
    }
    return variableIterator->second;
}

bool Scope::containsVariable(const std::string &name_) {
    return variables.find(name_) != variables.end();
}
