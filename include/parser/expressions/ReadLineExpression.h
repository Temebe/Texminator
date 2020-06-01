#ifndef TEXMINATOR_READLINEEXPRESSION_H
#define TEXMINATOR_READLINEEXPRESSION_H


#include "Expression.h"

class ReadLineExpression : public Expression, public OneFactorExpression {
public:
    explicit ReadLineExpression(std::unique_ptr<Expression> toRead_);
    Value evaluate(Environment &environment) override;

private:
    std::unique_ptr<Expression> toRead;
};


#endif //TEXMINATOR_READLINEEXPRESSION_H
