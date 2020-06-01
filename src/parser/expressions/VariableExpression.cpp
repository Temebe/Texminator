#include "parser/expressions/VariableExpression.h"

#include <utility>

VariableExpression::VariableExpression(std::string name_)
    : name(std::move(name_)) {}

// getVariable will throw if it won't succeed
Value VariableExpression::evaluate(Environment &environment) {
    return environment.getVariable(name).value();
}

const std::string &VariableExpression::getName() const {
    return name;
}
