#ifndef TEXMINATOR_READCHAREXPRESSION_H
#define TEXMINATOR_READCHAREXPRESSION_H


#include "Expression.h"

class ReadCharExpression : public Expression {
public:
    explicit ReadCharExpression(std::unique_ptr<Expression> toRead_);
    Value evaluate(Environment &environment) override;

private:
    std::unique_ptr<Expression> toRead;
};


#endif //TEXMINATOR_READCHAREXPRESSION_H
