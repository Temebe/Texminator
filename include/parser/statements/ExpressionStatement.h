#ifndef TEXMINATOR_EXPRESSIONSTATEMENT_H
#define TEXMINATOR_EXPRESSIONSTATEMENT_H


#include <parser/expressions/Expression.h>
#include "Statement.h"

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(std::unique_ptr<Expression> exp_)
        : expression(std::move(exp_)) {}
    void execute(Environment& environment) override;

private:
    std::unique_ptr<Expression> expression;
};


#endif //TEXMINATOR_EXPRESSIONSTATEMENT_H
