#include "parser/ParserException.h"
#include "parser/Environment.h"

#include <algorithm>

const std::string Environment::returnValueName = "#return";

void Environment::createNewScope(const ScopeType type) {
    scopes.push_front(Scope(type));
}

// TODO Why do I return optional if I throw exception anyway? Refactor this
std::optional<Value> Environment::getVariable(const std::string &name_) {
    auto currentScope = scopes.begin();
    std::optional<Value> result;

    while (currentScope != scopes.end()) {
        result = currentScope->getVariable(name_);
        if ((currentScope++)->getType() == ScopeType::function || result) {
            break;
        }
    }

    if (!result) {
        result = globalScope.getVariable(name_);
    }

    if (result) {
        return result;
    }

    throw ParserException("Variable " + name_ + " does not exist");
}


void Environment::addVariable(const std::string &name_, const Value &variable_) {
    auto currentScope = scopes.begin();
    currentScope->addVariable(name_, variable_);
}

void Environment::addFunction(const std::string &name_, Function &function_) {
    auto currentScope = scopes.begin();
    currentScope->addFunction(name_, function_);
}

void Environment::addGlobalVariable(const std::string &name_, const Value &variable_) {
    globalScope.addVariable(name_, variable_);
}

void Environment::addGlobalFunction(const std::string &name_, Function &function_) {
    globalScope.addFunction(name_, function_);
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

    if (globalScope.containsFunction(name_, parameters_)) {
        return globalScope.getFunction(name_, parameters_);
    }

    throw ParserException("Function " + name_ + " with given parameters does not exist");
}

void Environment::setVariable(const std::string &name_, const Value &variable_) {
    auto currentScope = scopes.begin();

    while (currentScope != scopes.end()) {
        if (auto currentVariable = currentScope->getVariable(name_)) {
            auto varType = static_cast<ValueEnum>(currentVariable->index());
            currentScope->replaceVariable(name_, castValue(variable_, varType));
            return;
        }

        if ((currentScope++)->getType() == ScopeType::function) {
            break;
        }
    }

    throw ParserException("Variable " + name_ + " does not exist");
}

void Environment::setReturnValue(const Value &value_) {
    auto functionScope = std::find_if(scopes.begin(), scopes.end(),
            [](const Scope& scope) { return scope.getType() == function; });

    if (functionScope == scopes.end()) {
        throw TexminatorException("Tried to return not from function!");
    }

    functionScope->addVariable(returnValueName, value_);
}

Value Environment::getReturnValue() {
    auto functionScope = std::find_if(scopes.begin(), scopes.end(),
                                      [](const Scope& scope) { return scope.getType() == function; });

    if (functionScope == scopes.end()) {
        throw TexminatorException("Tried to return not from function!");
    }

    auto returnValue = functionScope->getVariable(returnValueName);
    if (!returnValue) {
        throw TexminatorException("Could not find a return value");
    }

    return *returnValue;
}
