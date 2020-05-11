#include <parser/statements/Statement.h>
#include <iostream>
#include <parser/statements/VariableDeclarationStatement.h>
#include <parser/expressions/LiteralExpression.h>
#include <parser/expressions/VariableExpression.h>
#include <parser/expressions/FunctionCallExpression.h>
#include <stack>
#include <parser/expressions/ExpressionStack.h>
#include <parser/statements/OpenStatement.h>
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
                return parseVariableDeclaration(scanner_, ValueEnum::BOOL);
            }

        case constHornerHash("char"):
            if (token.value == "char") {
                return parseVariableDeclaration(scanner_, ValueEnum::CHAR);
            }

        case constHornerHash("float"):
            if (token.value == "float") {
                return parseVariableDeclaration(scanner_, ValueEnum::FLOAT);
            }

        case constHornerHash("unsigned"):
            if (token.value == "unsigned") {
                return parseVariableDeclaration(scanner_, ValueEnum::UNSIGNED_NUMBER);
            }

        case constHornerHash("number"):
            if (token.value == "number") {
                return parseVariableDeclaration(scanner_, ValueEnum::NUMBER);
            }

        case constHornerHash("string"):
            if (token.value == "string") {
                return parseVariableDeclaration(scanner_, ValueEnum::BOOL);
            }

        case constHornerHash("open"):
            if (token.value == "open") {
                return parseOpenStatement(scanner_);
            }


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

std::unique_ptr<Statement> Parser::parseOpenStatement(Scanner &scanner_) {
    auto token = scanner_.consume();
    OpenMode mode = readwrite;
    std::string alias;
    std::unique_ptr<Expression> nameExp;

    if (token.type == keyword && token.value == "to") {
        token = scanner_.consume();

        if (token.type == keyword && token.value == "read") {
            mode = read;
        } else if (token.type == keyword && token.value == "write") {
            mode = write;
        } else {
            setError("Expected mode specifier after \"to\" keyword", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }

        token = scanner_.consume();
    }

    if (!(nameExp = parseSimpleExpression(scanner_))) {
        setError("Expected expression with file name", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.getCurrentToken();
    if (token.type == keyword && token.value == "as") {
        token = scanner_.consume();
        if (token.type != identifier) {
            setError("Expected identifier after \"as\" keyword", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }
        alias = token.value;
        scanner_.consume();
    }

    return std::make_unique<OpenStatement>(std::move(nameExp), alias, mode);
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
            scanner_.consume();
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
            scanner_.consume();
            break;

        default:
            break;

    }

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
    token = scanner_.consume();

    if (token.type != leftRoundBracket) {
        return std::make_unique<VariableExpression>(name);
    }

    scanner_.consume(); // skip first bracket indicating parameters
    std::list<std::unique_ptr<Expression>> args;
    // Move token from left bracket and consume expressions as long as there is no right bracket
    while (token.type != rightRoundBracket && token.type != fileEnd) {
        auto compExp = parseCompoundExpression(scanner_, true);
        token = scanner_.getCurrentToken();
        if (compExp) {
            args.push_back(std::move(compExp));
        } else {
            break;
        }
    }

    if (scanner_.getCurrentToken().type == rightRoundBracket) {
        scanner_.consume();
        return std::make_unique<FunctionCallExpression>(name, std::move(args));
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
        if (stopOnRoundBracket_ && token.type == rightRoundBracket && leftBrackets == 1) {
            break;
        }

        switch (token.type) {
            case leftRoundBracket:
                expStack.pushOperator(token.type);
                leftBrackets += 1;
                token = scanner_.consume();
                break;

            case rightRoundBracket:
                expStack.pushOperator(token.type);
                leftBrackets -= 1;
                token = scanner_.consume();
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
                token = scanner_.consume();
                break;

            case numericLiteral:
            case floatLiteral:
            case stringLiteral:
            case identifier:
            case keyword:
                // if addExpression returns false that means that parsing of a simple expression failed, therefore
                // it is assumed that compound expression is over
                if (!expStack.addExpression(parseSimpleExpression(scanner_))) {
                    return expStack.calculateExpression();
                }
                token = scanner_.getCurrentToken();
                break;

            // If encountered token which cannot be parsed to an expression then calculate an expression
            // from what has been accumulated
            default:
                return expStack.calculateExpression();
        }
    }

    return expStack.calculateExpression();
}
