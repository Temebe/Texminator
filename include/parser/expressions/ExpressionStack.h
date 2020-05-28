#ifndef TEXMINATOR_EXPRESSIONSTACK_H
#define TEXMINATOR_EXPRESSIONSTACK_H


#include <Token.h>
#include <deque>
#include "Expression.h"

/**
 * Interface for Reverse Polish Notation
 *
 * TODO explain
 */
class ExpressionStack {
public:
    void pushOperator(TokenType token_);
    bool addExpression(std::unique_ptr<Expression> expression_);
    TokenType popOperator();
    std::unique_ptr<Expression> calculateExpression();

private:
    static int getPriority(TokenType token_);
    static std::unique_ptr<Expression> createExpression(TokenType type_);

    std::deque<TokenType> operatorStack;
    std::deque<std::unique_ptr<Expression>> expressionsStack;
};


#endif //TEXMINATOR_EXPRESSIONSTACK_H
