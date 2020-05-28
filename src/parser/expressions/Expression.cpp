#include "parser/expressions/Expression.h"

void TwoFactorExpression::setLeftExpression(std::unique_ptr<Expression> exp_) {
    leftExpression = std::move(exp_);
}

void TwoFactorExpression::setRightExpression(std::unique_ptr<Expression> exp_) {
    rightExpression = std::move(exp_);
}

const std::unique_ptr<Expression> &TwoFactorExpression::getLeftExpression() const {
    return leftExpression;
}

const std::unique_ptr<Expression> &TwoFactorExpression::getRightExpression() const {
    return rightExpression;
}

Expression::~Expression() = default;

void OneFactorExpression::setFactorExpression(std::unique_ptr<Expression> exp_) {
    factorExpression = std::move(exp_);
}

const std::unique_ptr<Expression> &OneFactorExpression::getFactorExpression() const {
    return factorExpression;
}
