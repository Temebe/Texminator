#include <algorithm>
#include "parser/Scope.h"

#include "parser/ParserException.h"


Scope::Scope(const ScopeType type_)
    : type(type_) {}

ScopeType Scope::getType() {
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

bool Scope::containsFunction(const std::string &name_, const std::list<Parameter> &parameters_) const {
    return findFunction(name_, parameters_) == functions.cend();
}

const Function& Scope::getFunction(const std::string &name_, const std::list<Parameter> &parameters_) const {
    auto result = findFunction(name_, parameters_);
    if (result == functions.cend()) {
        throw ParserException("Function " + name_ + " with given parameters does not exist");
    }
    return result->second;
}

FunctionMap::const_iterator Scope::findFunction(const std::string &name_, const std::list<Parameter> &parameters_) const {
    auto [begin, end] = functions.equal_range(name_);
    if (end != functions.end()) {
        ++end;
    }

    // TODO implement
    return std::find_if(begin, end,
        [&name_, &parameters_](const auto &element_){
            return true;
        });
}
