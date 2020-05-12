#ifndef TEXMINATOR_EXPRESSIONSTATEMENT_H
#define TEXMINATOR_EXPRESSIONSTATEMENT_H

#include <Token.h>
#include <parser/expressions/Expression.h>
#include "Statement.h"

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(std::string identifier_, TokenType operationType_,
                        std::unique_ptr<Expression> expression_);
    void execute(Environment& environment) override;

private:
    const std::string identifier;
    const TokenType operationType;
    const std::unique_ptr<Expression> expression;
};


#endif //TEXMINATOR_EXPRESSIONSTATEMENT_H
