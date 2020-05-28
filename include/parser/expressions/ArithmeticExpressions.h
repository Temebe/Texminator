#ifndef TEXMINATOR_ARITHMETICEXPRESSIONS_H
#define TEXMINATOR_ARITHMETICEXPRESSIONS_H

#include "Expression.h"

// TODO Create ArithmeticExpression class which will bring to a common some lines of code

class AdditionExpression : public Expression, public TwoFactorExpression {
public:
    explicit AdditionExpression(bool subtraction_ = false);
    Value evaluate(Environment &environment) override;

private:
    bool subtraction;
};

class MultiplicationExpression : public Expression, public TwoFactorExpression {
public:
    explicit MultiplicationExpression(bool division_ = false);
    Value evaluate(Environment &environment) override;

private:
    bool division;
};

class ModuloExpression : public Expression, public TwoFactorExpression {
public:
    Value evaluate(Environment &environment) override;
};

#endif //TEXMINATOR_ARITHMETICEXPRESSIONS_H
