#ifndef TEXMINATOR_INCREMENTLINEEXPRESSION_H
#define TEXMINATOR_INCREMENTLINEEXPRESSION_H


#include "Expression.h"

class IncrementLineExpression : public Expression, public OneFactorExpression  {
public:
    explicit IncrementLineExpression();
    Value evaluate(Environment &environment) override;

};


#endif //TEXMINATOR_INCREMENTLINEEXPRESSION_H
