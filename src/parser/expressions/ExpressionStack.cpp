#include <stack>
#include "parser/expressions/ExpressionStack.h"
#include "parser/expressions/ComparisonExpressions.h"
#include "parser/expressions/ArithmeticExpressions.h"
#include "parser/expressions/LogicalExpressions.h"

void ExpressionStack::pushOperator(TokenType token_) {
    if (getPriority(token_) == 0) {
        return;
    }

    if (token_ == rightRoundBracket) {
        while (!operatorStack.empty()) {
            auto op = popOperator();
            if (op == leftRoundBracket) {
                return;
            }
            addExpression(createExpression(op));
            return;
        }
    }

    if (token_ == leftRoundBracket) {
        operatorStack.push_front(token_);
        return;
    }

    while (!operatorStack.empty()) {
        if (getPriority(token_) <= getPriority(operatorStack.front())) {
            addExpression(createExpression(operatorStack.front()));
            operatorStack.pop_front();
            continue;
        }
        break;
    }
    operatorStack.push_front(token_);
}

TokenType ExpressionStack::popOperator() {
    auto result = *operatorStack.begin();
    operatorStack.pop_front();
    return result;
}

int ExpressionStack::getPriority(const TokenType token_) {
    switch (token_) {
        case leftRoundBracket:
        case rightRoundBracket:
            return 8;

        case notOperator:
            return 7;

        case mulOperator:
        case divOperator:
        case modOperator:
            return 6;

        case plus:
        case subOperator:
            return 5;

        case ltOperator:
        case leOperator:
        case gtOperator:
        case geOperator:
            return 4;

        case eqOperator:
        case neOperator:
            return 3;

        case andOperator:
            return 2;

        case orOperator:
            return 1;

        default:
            return 0;
    }
}

void ExpressionStack::addExpression(std::unique_ptr<Expression> expression_) {
    expressionsList.push_back(std::move(expression_));
}

std::unique_ptr<Expression> ExpressionStack::createExpression(TokenType type_) {
    switch (type_) {
        case mulOperator:
            return std::make_unique<MultiplicationExpression>();
        case divOperator:
            return std::make_unique<MultiplicationExpression>(true);
        case modOperator:
            return std::make_unique<ModuloExpression>();
        case plus:
            return std::make_unique<AdditionExpression>();
        case subOperator:
            return std::make_unique<AdditionExpression>(true);
        case ltOperator:
            return std::make_unique<RelationalExpression>(RelationType::lesser);
        case leOperator:
            return std::make_unique<RelationalExpression>(RelationType::lesserEqual);
        case gtOperator:
            return std::make_unique<RelationalExpression>(RelationType::greater);
        case geOperator:
            return std::make_unique<RelationalExpression>(RelationType::greaterEqual);
        case eqOperator:
            return std::make_unique<EqualExpression>();
        case neOperator:
            return std::make_unique<EqualExpression>(true);
        case andOperator:
            return std::make_unique<AndExpression>();
        case orOperator:
            return std::make_unique<OrExpression>();
        case notOperator:
            return std::make_unique<NotExpression>();
        default:
            return std::unique_ptr<Expression>();
    }
}

// TODO Any better way than raw pointer cast?
/**
 *
 * @return expression
 */
std::unique_ptr<Expression> ExpressionStack::calculateExpression() {
    // First read all remaining operators from stack
    while (!operatorStack.empty()) {
        addExpression(createExpression(operatorStack.front()));
        operatorStack.pop_front();
    }

    std::stack<std::unique_ptr<Expression>> stack;
    while (!expressionsList.empty()) {
        std::unique_ptr<Expression> exp = std::move(expressionsList.front());
        expressionsList.pop_front();

        if (auto twoFactorExp = dynamic_cast<TwoFactorExpression*>(exp.get())) {
            auto rightExpression = std::move(stack.top());
            stack.pop();
            auto leftExpression = std::move(stack.top());
            stack.pop();
            twoFactorExp->setLeftExpression(std::move(leftExpression));
            twoFactorExp->setRightExpression(std::move(rightExpression));
        }

        stack.push(std::move(exp));
    }

    if (stack.size() != 1) {
        // TODO handle errors in reverse polish notation implementation
    }

    return std::move(stack.top());
}

