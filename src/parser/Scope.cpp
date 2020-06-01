#include <algorithm>
#include "parser/Scope.h"

#include "parser/ParserException.h"


Scope::Scope(const ScopeType type_)
    : type(type_) {}

ScopeType Scope::getType() const {
    return type;
}

std::optional<Value> Scope::getVariable(const std::string &name_) const {
    auto variableIterator = variables.find(name_);
    if (variableIterator == variables.end()) {
        return std::optional<Value>();
    }
    return variableIterator->second;
}

bool Scope::containsVariable(const std::string &name_) const {
    return variables.find(name_) != variables.end();
}

void Scope::addVariable(const std::string &name_, const Value &variable_) {
    if (containsVariable(name_)) {
        throw ParserException("Unable to add " + name_ + " variable to a scope - name already taken.");
    }

    variables.insert({name_, variable_});
}

void Scope::replaceVariable(const std::string &name_, const Value& variable_) {
    if (!containsVariable(name_)) {
        throw ParserException("Unable to add " + name_ + " variable to a scope - cannot find variable.");
    }

    variables.insert_or_assign(name_, variable_);
}

void Scope::addFunction(const std::string &name_, Function &function_) {
    functions.insert({name_, std::move(function_)});
}

bool Scope::containsFunction(const std::string &name_, const std::list<Parameter> &parameters_) {
    return findFunction(name_, parameters_) != functions.cend();
}

Function& Scope::getFunction(const std::string &name_, const std::list<Parameter> &parameters_) {
    auto result = findFunction(name_, parameters_);
    if (result == functions.end()) {
        throw ParserException("Function " + name_ + " with given parameters does not exist");
    }
    return result->second;
}

FunctionMap::iterator Scope::findFunction(const std::string &name_, const std::list<Parameter> &parameters_) {
    auto[it, end] = functions.equal_range(name_);

    while (it != end && it != functions.end()) {
        if (it->second.hasSameParameters(parameters_)) {
            return it;
        }
        ++it;
    }
    return functions.end();
}
