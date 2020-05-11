#include "parser/expressions/LiteralExpression.h"

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

}

void LiteralExpression::initializeFloat(const std::string &val_) {

}

void LiteralExpression::initializeNumber(const std::string &val_) {

}
