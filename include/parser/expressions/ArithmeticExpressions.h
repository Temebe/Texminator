#ifndef TEXMINATOR_ARITHMETICEXPRESSIONS_H
#define TEXMINATOR_ARITHMETICEXPRESSIONS_H

#include "Expression.h"

class AdditionExpression : public Expression, public TwoFactorExpression {
public:
    explicit AdditionExpression(bool subtraction = false);
    Value evaluate(Environment &environment) override;
};

class MultiplicationExpression : public Expression, public TwoFactorExpression {
public:
    explicit MultiplicationExpression(bool division = false);
    Value evaluate(Environment &environment) override;
};

class ModuloExpression : public Expression, public TwoFactorExpression {
public:
    Value evaluate(Environment &environment) override;
};

#endif //TEXMINATOR_ARITHMETICEXPRESSIONS_H
