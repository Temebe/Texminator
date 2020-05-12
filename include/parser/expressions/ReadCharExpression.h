#ifndef TEXMINATOR_READCHAREXPRESSION_H
#define TEXMINATOR_READCHAREXPRESSION_H


#include "Expression.h"

class ReadCharExpression : public Expression, public OneFactorExpression  {
public:
    explicit ReadCharExpression(std::unique_ptr<Expression> toIncrement_);
    Value evaluate(Environment &environment) override;

private:
    std::unique_ptr<Expression> toIncrement;
};


#endif //TEXMINATOR_READCHAREXPRESSION_H
