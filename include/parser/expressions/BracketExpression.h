#ifndef TEXMINATOR_BRACKETEXPRESSION_H
#define TEXMINATOR_BRACKETEXPRESSION_H


#include "Expression.h"

/**
 * Any expression wrapped in parenthesis
 */
class BracketExpression : public Expression {
public:
    explicit BracketExpression(std::unique_ptr<Expression> expression);
    Value evaluate(Environment &environment) override;

private:
    std::unique_ptr<Expression> expression;
};


#endif //TEXMINATOR_BRACKETEXPRESSION_H
