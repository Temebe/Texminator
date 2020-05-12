#ifndef TEXMINATOR_EXPRESSION_H
#define TEXMINATOR_EXPRESSION_H

#include "parser/Environment.h"
#include "parser/Value.h"

class Expression {
public:
    virtual ~Expression();
    virtual Value evaluate(Environment &environment) = 0;
};

class TwoFactorExpression {
public:
    void setLeftExpression(std::unique_ptr<Expression> exp_);
    void setRightExpression(std::unique_ptr<Expression> exp_);

protected:
    std::unique_ptr<Expression> leftExpression;
    std::unique_ptr<Expression> rightExpression;
};

class OneFactorExpression {
public:
    void setFactorExpression(std::unique_ptr<Expression> exp_);

protected:
    std::unique_ptr<Expression> factorExpression;
};

#endif //TEXMINATOR_EXPRESSION_H
