#include "parser/expressions/LiteralExpression.h"

LiteralExpression::LiteralExpression(bool val_) {
    val.setVal(val_);
}

LiteralExpression::LiteralExpression(const Token &token_) {
    if (token_.type == stringLiteral) {
        initializeString(token_.value);

    } else if (token_.type == floatLiteral) {
        initializeFloat(token_.value);

    } else if (token_.type == numericLiteral) {
        initializeNumber(token_.value);
    }
}

Value LiteralExpression::evaluate(Environment &environment) {
    return val;
}

void LiteralExpression::initializeString(const std::string &val_) {
    val.setVal(val_);
}

void LiteralExpression::initializeFloat(const std::string &val_) {
    ValueType floatNumber = static_cast<FloatType>(std::stod(val_));
    val.setVal(floatNumber);
}

void LiteralExpression::initializeNumber(const std::string &val_) {
    ValueType number;
    if (*val_.begin() == '-') {
        number = static_cast<NumberType>(std::stoll(val_));
    } else {
        number = static_cast<UnsignedNumberType>(std::stoull(val_));
    }
    val.setVal(number);
}

