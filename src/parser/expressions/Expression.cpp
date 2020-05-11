#include "parser/expressions/Expression.h"

void TwoFactorExpression::setLeftExpression(std::unique_ptr<Expression> exp_) {
    leftExpression = std::move(exp_);
}

void TwoFactorExpression::setRightExpression(std::unique_ptr<Expression> exp_) {
    rightExpression = std::move(exp_);
}

Expression::~Expression() = default;
