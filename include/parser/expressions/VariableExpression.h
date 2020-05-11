#ifndef TEXMINATOR_VARIABLEEXPRESSION_H
#define TEXMINATOR_VARIABLEEXPRESSION_H


#include "Expression.h"

class VariableExpression : public Expression {
public:
    explicit VariableExpression(std::string name_);
    Value evaluate(Environment &environment) override;

private:
    std::string name;
};


#endif //TEXMINATOR_VARIABLEEXPRESSION_H
