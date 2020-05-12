#ifndef TEXMINATOR_INCREMENTCHAREXPRESSION_H
#define TEXMINATOR_INCREMENTCHAREXPRESSION_H


#include "Expression.h"

class IncrementCharExpression : public Expression, public OneFactorExpression {
public:
    explicit IncrementCharExpression();
    Value evaluate(Environment &environment) override;
};


#endif //TEXMINATOR_INCREMENTCHAREXPRESSION_H
