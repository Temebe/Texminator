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

private:
    std::unique_ptr<Expression> leftExpression;
    std::unique_ptr<Expression> rightExpression;
};

#endif //TEXMINATOR_EXPRESSION_H
