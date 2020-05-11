#ifndef TEXMINATOR_LOGICALEXPRESSIONS_H
#define TEXMINATOR_LOGICALEXPRESSIONS_H


#include "Expression.h"

class AndExpression : public Expression, public TwoFactorExpression {
public:
    Value evaluate(Environment &environment) override;
};

class OrExpression : public Expression, public TwoFactorExpression {
public:
    Value evaluate(Environment &environment) override;
};

class NotExpression : public Expression, public TwoFactorExpression {
public:
    Value evaluate(Environment &environment) override;
};


#endif //TEXMINATOR_LOGICALEXPRESSIONS_H
