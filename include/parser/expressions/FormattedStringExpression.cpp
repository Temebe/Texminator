#include "FormattedStringExpression.h"

FormattedStringExpression::FormattedStringExpression(std::unique_ptr<Expression> string_,
                                                     std::list<std::unique_ptr<Expression>> arguments_)
                                                     : stringExpression(std::move(string_)),
                                                       arguments(std::move(arguments_)) {}

Value FormattedStringExpression::evaluate(Environment &environment) {
    return Value();
}


