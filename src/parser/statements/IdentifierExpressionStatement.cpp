#include "parser/statements/IdentifierExpressionStatement.h"

#include "TexminatorExceptions.h"
#include "parser/expressions/ArithmeticExpressions.h"
#include "parser/expressions/LiteralExpression.h"

IdentifierExpressionStatement::IdentifierExpressionStatement(std::string identifier_, TokenType operationType_,
                                                             std::unique_ptr<Expression> expression_)
                                         : identifier(std::move(identifier_)),
                                           operationType(operationType_),
                                           expression(std::move(expression_)) {}

void IdentifierExpressionStatement::execute(Environment &environment) {
    auto currentVal = environment.getVariable(identifier);
    if (!currentVal) {
        throw NonExistentVariableException(identifier);
    }

    auto expValue = expression->evaluate(environment);
    std::unique_ptr<Expression> operation;

    switch (operationType) {
        case assignOperator:
            environment.setVariable(identifier, expValue);
            return;

        case addAssignOperator:
            operation = createAdditionStatement(currentVal.value(), expValue);
            break;

        case subAssignOperator:
            operation = createSubtractionStatement(currentVal.value(), expValue);
            break;

        case divAssignOperator:
            operation = createDivisionStatement(currentVal.value(), expValue);
            break;

        case mulAssignOperator:
            operation = createMultiplicationStatement(currentVal.value(), expValue);
            break;

        default:
            throw TexminatorException("Expression statement received wrong assignment symbol");
    }

    if (!operation) {
        throw TexminatorException("Could not execute properly expression statement");
    }
    environment.setVariable(identifier, operation->evaluate(environment));
}

std::unique_ptr<Expression> IdentifierExpressionStatement::createAdditionStatement(const Value &l_, const Value &r_,
                                                                                   bool subtraction) {
    auto operation = std::make_unique<AdditionExpression>(subtraction);
    operation->setLeftExpression(std::make_unique<LiteralExpression>(l_));
    operation->setRightExpression(std::make_unique<LiteralExpression>(r_));
    return operation;
}

std::unique_ptr<Expression> IdentifierExpressionStatement::createSubtractionStatement(const Value &l_, const Value &r_) {
    return createAdditionStatement(l_, r_, true);
}

std::unique_ptr<Expression> IdentifierExpressionStatement::createMultiplicationStatement(const Value &l_, const Value &r_,
                                                                                         bool division) {
    auto operation = std::make_unique<MultiplicationExpression>(division);
    operation->setLeftExpression(std::make_unique<LiteralExpression>(l_));
    operation->setRightExpression(std::make_unique<LiteralExpression>(r_));
    return operation;
}

std::unique_ptr<Expression> IdentifierExpressionStatement::createDivisionStatement(const Value &l_, const Value &r_) {
    return createMultiplicationStatement(l_, r_, true);
}
