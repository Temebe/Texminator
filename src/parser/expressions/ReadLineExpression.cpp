#include "parser/expressions/ReadLineExpression.h"

ReadLineExpression::ReadLineExpression(std::unique_ptr<Expression> toIncrement_)
    : toIncrement(std::move(toIncrement_)){}

Value ReadLineExpression::evaluate(Environment &environment) {
    return Value();
}
