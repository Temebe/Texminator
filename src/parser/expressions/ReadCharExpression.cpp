#include "parser/expressions/ReadCharExpression.h"

ReadCharExpression::ReadCharExpression(std::unique_ptr<Expression> toIncrement_)
    : toIncrement(std::move(toIncrement_)){}

Value ReadCharExpression::evaluate(Environment &environment) {
    return Value();
}
