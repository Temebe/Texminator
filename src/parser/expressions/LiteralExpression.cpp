#include "parser/expressions/LiteralExpression.h"

#include <utility>

LiteralExpression::LiteralExpression(bool val_) {
    val = val_;
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

LiteralExpression::LiteralExpression(Value val_) : val(std::move(val_)) {}

Value LiteralExpression::evaluate(Environment &environment) {
    return val;
}

void LiteralExpression::initializeString(const std::string &val_) {
    if (val_.size() == 1) { // save it as an char if it has one letter
        val = *val_.begin();
        return;
    }
    val = val_;
}

void LiteralExpression::initializeFloat(const std::string &val_) {
    Value floatNumber = static_cast<FloatType>(std::stod(val_));
    val = floatNumber;
}

void LiteralExpression::initializeNumber(const std::string &val_) {
    if (*val_.begin() == '-') {
        val = static_cast<NumberType>(std::stoll(val_));
    } else {
        val = static_cast<UnsignedNumberType>(std::stoull(val_));
    }
}
