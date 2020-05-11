#include "parser/expressions/FunctionCallExpression.h"

#include <utility>

Value FunctionCallExpression::evaluate(Environment &environment) {
    return Value();
}

FunctionCallExpression::FunctionCallExpression(std::string name_, std::list<std::unique_ptr<Expression>> arguments_)
    : name(std::move(name_)),
      arguments(std::move(arguments_)) {}
