#include <parser/statements/Statement.h>
#include <iostream>
#include <parser/statements/VariableDeclarationStatement.h>
#include <parser/expressions/LiteralExpression.h>
#include <parser/expressions/VariableExpression.h>
#include <parser/expressions/FunctionCallExpression.h>
#include <stack>
#include <parser/expressions/ExpressionStack.h>
#include "parser/Parser.h"
#include "HornerHash.h"

void Parser::parse(Scanner &scanner_) {
    initializeFirstScope();
    environment.createNewScope(local);
    std::unique_ptr<Statement> statement;

    for (Token token = scanner_.getCurrentToken();
         token.type != unknown && token.type != fileEnd;
         token = scanner_.consume()) {

        switch (token.type) {
            case identifier:
                statement = parseAfterIdentifier(scanner_);
                break;

            case keyword:
                statement = parseAfterKeyword(scanner_);
                break;

            default:
                break;
        }

        if (statement) {
            statement->execute(environment);
        } else {
            printErrorMsg();
            break;
        }
    }

    environment.destroyCurrentScope();
}

// TODO Maybe connect somehow keywords from scanner to these
std::unique_ptr<Statement> Parser::parseAfterKeyword(Scanner &scanner_) {
    auto token = scanner_.getCurrentToken();
    switch (hornerHash(token.value.c_str())) {
        case constHornerHash("bool"):
            if (token.value == "bool") {
                parseVariableDeclaration(scanner_, ValueEnum::BOOL);
            }
            break;

        case constHornerHash("char"):
            if (token.value == "char") {
                parseVariableDeclaration(scanner_, ValueEnum::CHAR);
            }
            break;

        case constHornerHash("float"):
            if (token.value == "float") {
                parseVariableDeclaration(scanner_, ValueEnum::FLOAT);
            }
            break;

        case constHornerHash("unsigned"):
            if (token.value == "unsigned") {
                parseVariableDeclaration(scanner_, ValueEnum::UNSIGNED_NUMBER);
            }
            break;

        case constHornerHash("number"):
            if (token.value == "number") {
                parseVariableDeclaration(scanner_, ValueEnum::NUMBER);
            }
            break;

        case constHornerHash("string"):
            if (token.value == "string") {
                parseVariableDeclaration(scanner_, ValueEnum::BOOL);
            }
            break;


        default:
            break;
    }
}

std::unique_ptr<Statement> Parser::parseAfterIdentifier(Scanner &scanner_) {

}

void Parser::initializeFirstScope() {

}

std::unique_ptr<Statement> Parser::parseVariableDeclaration(Scanner &scanner_, ValueEnum type) {
    Token token = scanner_.consume();
    // If there is word "number" after "unsigned" just skip it
    if (type == UNSIGNED_NUMBER && token.type == keyword && token.value == "number") {
        token = scanner_.consume();
    }
    if (token.type != identifier) {
        setError("Expected identifier", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    auto statement = std::make_unique<VariableDeclarationStatement>(token.value, type);
    token = scanner_.consume();
    if (token.type == semicolon) {
        return statement;
    }

    if (token.type != assignOperator) {
        setError("Expected assignment operator", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume();
    auto expression = parseSimpleExpression(scanner_);
    if (!expression) {
        setError("Expected expression here", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    statement->setAssignmentExpression(std::move(expression));
    return statement;
}

void Parser::setError(const std::string &err_, const unsigned int line_, const unsigned int pos_) {
    ParseError error;
    error.err = err_;
    error.line = line_;
    error.pos = pos_;

    errors.push_back(error);
}

void Parser::printErrorMsg() const {
    std::cerr << errors.size() << " errors occurred while parsing:" << std::endl;

    std::cerr << std::endl;
    for (const ParseError& error : errors) {
        std::cerr << "Line " + std::to_string(error.line) << ", pos " + std::to_string(error.pos) << ":" << std::endl;
        std::cerr << error.err << std::endl << std::endl;
    }
}

std::unique_ptr<Expression> Parser::parseSimpleExpression(Scanner &scanner_) {
    std::unique_ptr<Expression> leftExpression;
    Token token = scanner_.getCurrentToken();
    switch (token.type) {
        case numericLiteral:
        case floatLiteral:
        case stringLiteral:
            leftExpression = std::make_unique<LiteralExpression>(token);
            break;

        case identifier:
            leftExpression = parseVariableOrFunctionExpression(scanner_);
            break;

        case keyword:
            if (token.value == "true") {
                leftExpression = std::make_unique<LiteralExpression>(true);
            } else if (token.value == "false") {
                leftExpression = std::make_unique<LiteralExpression>(false);
            }
            break;

        default:
            break;

    }

    scanner_.consume();
    return leftExpression;
}

std::unique_ptr<Expression> Parser::parseVariableOrFunctionExpression(Scanner &scanner_) {
    auto token = scanner_.getCurrentToken();
    const auto firstToken = scanner_.getCurrentToken();

    if (token.type != identifier) {
        setError("Expected identifier", token.line, token.pos);
        return std::unique_ptr<Expression>();
    }
    std::string name = token.value;

    if (scanner_.peek().type != leftRoundBracket) {
        return std::make_unique<VariableExpression>(name);
    }

    std::list<std::unique_ptr<Expression>> args;
    // Move token from left bracket and consume expressions as long as there is no right bracket
    while (scanner_.consume().type != rightRoundBracket && scanner_.getCurrentToken().type != fileEnd) {
        args.push_back(parseCompoundExpression(scanner_, comma));
    }
    if (scanner_.getCurrentToken().type == rightRoundBracket) {
        scanner_.consume();
        return std::make_unique<FunctionCallExpression>(name, args);
    }

    setError("Expected list of arguments ", firstToken.line, firstToken.pos);
    return std::unique_ptr<Expression>();
}

// TODO Change this while to something readable
std::unique_ptr<Expression> Parser::parseCompoundExpression(Scanner &scanner_, const bool stopOnRoundBracket_) {
    int leftBrackets = 0;
    // Since brackets may be used in expressions to separate one from another we need to distinguish
    // priority brackets from closing bracket
    if (stopOnRoundBracket_) {
        leftBrackets += 1;
    }

    ExpressionStack expStack;
    auto token = scanner_.getCurrentToken();
    while (token.type != unknown && token.type != fileEnd) {
        if (token.type == rightRoundBracket && stopOnRoundBracket_ && leftBrackets == 1) {
            break;
        }

        switch (token.type) {
            case leftRoundBracket:
                expStack.pushOperator(token.type);
                leftBrackets += 1;
                break;

            case rightRoundBracket:
                expStack.pushOperator(token.type);
                leftBrackets -= 1;
                break;

            case notOperator:
            case mulOperator:
            case divOperator:
            case modOperator:
            case plus:
            case subOperator:
            case ltOperator:
            case leOperator:
            case gtOperator:
            case geOperator:
            case eqOperator:
            case neOperator:
            case andOperator:
            case orOperator:
                expStack.pushOperator(token.type);
                break;

            case numericLiteral:
            case floatLiteral:
            case stringLiteral:
            case identifier:
            case keyword:
                expStack.addExpression(parseSimpleExpression(scanner_));
                break;

            // If encountered token which cannot be parsed to an expression then calculate an expression
            // from what has been accumulated
            default:
                return expStack.calculateExpression();
        }
    }

    return expStack.calculateExpression();
}
