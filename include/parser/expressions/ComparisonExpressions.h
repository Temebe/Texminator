#ifndef TEXMINATOR_COMPARISONEXPRESSIONS_H
#define TEXMINATOR_COMPARISONEXPRESSIONS_H

#include "Expression.h"


class EqualExpression : public Expression, public TwoFactorExpression {
public:
    explicit EqualExpression(bool negation_ = false) : negation(negation_) {};
    Value evaluate(Environment &environment) override;

private:
    const bool negation;
};


enum RelationType {
    greaterEqual, greater, lesser, lesserEqual
};

class RelationalExpression : public Expression, public TwoFactorExpression {
public:
    explicit RelationalExpression(RelationType type_) : relationType(type_) {};
    Value evaluate(Environment &environment) override;
private:
    RelationType relationType;
};

#endif //TEXMINATOR_COMPARISONEXPRESSIONS_H
