#ifndef TEXMINATOR_EXPRESSIONSTACK_H
#define TEXMINATOR_EXPRESSIONSTACK_H


#include <Token.h>
#include <deque>
#include <list>
#include "Expression.h"

/**
 * Interface for Reverse Polish Notation
 *
 * TODO explain
 */
class ExpressionStack {
public:
    void pushOperator(TokenType token_);
    void addExpression(std::unique_ptr<Expression> expression_);
    TokenType popOperator();
    std::unique_ptr<Expression> calculateExpression();

private:
    static int getPriority(TokenType token_);
    std::unique_ptr<Expression> createExpression(TokenType type_);
    void handleOperatorExpression(std::unique_ptr<TwoFactorExpression> exp_,
                                  std::stack<std::unique_ptr<Expression>>& stack_);

    std::list<TokenType> operatorStack;
    std::list<std::unique_ptr<Expression>> expressionsList;
};


#endif //TEXMINATOR_EXPRESSIONSTACK_H
