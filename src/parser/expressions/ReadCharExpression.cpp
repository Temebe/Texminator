#include "parser/expressions/ReadCharExpression.h"

ReadCharExpression::ReadCharExpression(std::unique_ptr<Expression> toRead_)
    : toRead(std::move(toRead_)){}

Value ReadCharExpression::evaluate(Environment &environment) {
    auto variable = toRead->evaluate(environment);
    if (!std::holds_alternative<StreamType>(variable)) {
        throw WrongTypeException("Tried to read from variable that is not a stream type");
    }

    auto result = std::get<StreamType>(variable);
    return result->readChar();
}
