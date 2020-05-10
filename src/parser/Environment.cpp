#include <parser/ParserException.h>
#include "parser/Environment.h"

void Environment::createNewScope(const ScopeType type) {
    scopes.push_front(Scope(type));
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

    throw ParserException("Variable " + name_ + " does not exist");
}

void Environment::destroyCurrentScope() {
    scopes.pop_front();
}

const Function& Environment::getFunction(const std::string &name_, const std::list<Parameter> &parameters_) {
    auto it = scopes.begin();
    for (; it != scopes.end(); ++it) {
        if (it->containsFunction(name_, parameters_)) {
            return it->getFunction(name_, parameters_);
        }
    }

    throw ParserException("Function " + name_ + " with given parameters does not exist");
}
