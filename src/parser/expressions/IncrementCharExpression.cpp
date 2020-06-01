#include "parser/expressions/IncrementCharExpression.h"

Value IncrementCharExpression::evaluate(Environment &environment) {
    auto variable = factorExpression->evaluate(environment);
    if (!std::holds_alternative<StreamType>(variable)) {
        throw WrongTypeException("Tried to read from variable that is not a stream type");
    }

    auto result = std::get<StreamType>(variable);
    result->readChar();
    return result;
}

IncrementCharExpression::IncrementCharExpression() {

}
