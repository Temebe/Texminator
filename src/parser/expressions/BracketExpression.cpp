#include "parser/expressions/BracketExpression.h"

BracketExpression::BracketExpression(std::unique_ptr<Expression> expression_)
    : expression(std::move(expression_)) {}

Value BracketExpression::evaluate(Environment &environment) {
    if (!expression) {
        return Value();
    }

    return expression->evaluate(environment);
}
