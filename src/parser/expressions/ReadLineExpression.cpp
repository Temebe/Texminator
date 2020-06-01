#include "parser/expressions/ReadLineExpression.h"

ReadLineExpression::ReadLineExpression(std::unique_ptr<Expression> toRead_)
    : toRead(std::move(toRead_)){}

// TODO ReadLineExpression and ReadCharExpression have some in common, maybe join them
Value ReadLineExpression::evaluate(Environment &environment) {
    auto variable = toRead->evaluate(environment);
    if (!std::holds_alternative<StreamType>(variable)) {
        throw WrongTypeException("Tried to read from variable that is not a stream type");
    }

    auto result = std::get<StreamType>(variable);
    return result->readLine();
}
