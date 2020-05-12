#include <parser/statements/Statement.h>
#include <iostream>
#include <parser/statements/VariableDeclarationStatement.h>
#include <parser/expressions/LiteralExpression.h>
#include <parser/expressions/VariableExpression.h>
#include <parser/expressions/FunctionCallExpression.h>
#include <stack>
#include <parser/expressions/ExpressionStack.h>
#include <parser/statements/OpenStatement.h>
#include <parser/statements/IfStatement.h>
#include <parser/statements/BlockStatement.h>
#include <parser/statements/AliasDeclarationStatement.h>
#include "parser/Parser.h"
#include "HornerHash.h"

void Parser::parse(Scanner &scanner_) {
    initializeFirstScope();
    environment.createNewScope(local);
    std::unique_ptr<Statement> statement;

    for (Token token = scanner_.getCurrentToken();
         token.type != unknown && token.type != fileEnd;) {

        statement = parseStatement(scanner_);

        if (statement) {
            statement->execute(environment);
        } else {
            printErrorMsg();
            break;
        }
    }

    environment.destroyCurrentScope();
}

std::unique_ptr<Statement> Parser::parseStatement(Scanner& scanner_) {
    Token token = scanner_.getCurrentToken();
    std::unique_ptr<Statement> statement;

    switch (token.type) {
        case identifier:
            statement = parseAfterIdentifier(scanner_);
            break;

        case keyword:
            statement = parseAfterKeyword(scanner_);
            break;

        case leftCurlyBracket:
            statement = parseBlockStatement(scanner_);
            break;

        default:
            break;
    }

    return statement;
}

// TODO Maybe connect somehow keywords from scanner to these
std::unique_ptr<Statement> Parser::parseAfterKeyword(Scanner &scanner_) {
    Token token = scanner_.getCurrentToken();
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

        case constHornerHash("if"):
            if (token.value == "if") {
                return parseIfStatement(scanner_);
            }

        case constHornerHash("use"):
            if (token.value == "use") {
                return parseAliasDeclaration(scanner_);
            }


        default:
            break;
    }
    return std::unique_ptr<Statement>();
}

std::unique_ptr<Statement> Parser::parseAfterIdentifier(Scanner &scanner_) {
    return std::unique_ptr<Statement>();
}

void Parser::initializeFirstScope() {

}


std::unique_ptr<Statement> Parser::parseBlockStatement(Scanner &scanner_) {
    Token token = scanner_.consume();
    std::list<std::unique_ptr<Statement>> statements;

    while (token.type != rightCurlyBracket && token.type != fileEnd && token.type != unknown) {
        auto statement = parseStatement(scanner_);
        if (!statement) {
            setError("Could not parse statement in statement block", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }
        token = scanner_.getCurrentToken();
    }

    if(token.type != rightCurlyBracket) {
        setError("Missing closure \"}\" of block statement", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip closing bracket
    return std::make_unique<BlockStatement>(std::move(statements));
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

    token = scanner_.getCurrentToken();
    if (token.type != semicolon) {
        setError("Expected semicolon here", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip semicolon
    return statement;
}


std::unique_ptr<Statement> Parser::parseAliasDeclaration(Scanner &scanner_) {
    auto token = scanner_.consume();
    std::string alias; // new identifier for variable
    std::string underlying; // identifier of variable that will be represented by alias

    if (token.type != identifier) {
        setError("Expected identifier after \"use\" keyword", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    underlying = token.value;
    token = scanner_.consume();
    if (!(token.type == keyword && token.value == "as")) {
        setError("Expected \"as\" keyword", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume();
    if (token.type != identifier ) {
        setError("Expected alias' identifier after \"as\" keyword", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    alias = token.value;

    token = scanner_.consume();
    if (token.type != semicolon ) {
        setError("Expected semicolon at the end of the statement", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip semicolon
    return std::make_unique<AliasDeclarationStatement>(alias, underlying);
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
        token = scanner_.consume();
    }

    if (token.type != semicolon) {
        setError("Semicolon missing", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip semicolon
    return std::make_unique<OpenStatement>(std::move(nameExp), alias, mode);
}


std::unique_ptr<Statement> Parser::parseIfStatement(Scanner &scanner_) {
    auto token = scanner_.consume();
    if (token.type != leftRoundBracket) {
        setError("Expected left bracket", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume(); // skin left bracket
    auto conditionExpression = parseCompoundExpression(scanner_, true);
    if (!conditionExpression) {
        setError("Expected condition for if statement", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    token = scanner_.getCurrentToken();
    if (token.type != rightRoundBracket) {
        setError("If condition not closed", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip closing bracket
    auto ifTrueStatement = parseStatement(scanner_);
    if (!ifTrueStatement) {
        setError("Expected statement after if", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.getCurrentToken();
    if (!(token.type == keyword && token.value == "else")) {
        return std::make_unique<IfStatement>(std::move(conditionExpression), std::move(ifTrueStatement));
    }

    scanner_.consume(); // skip else keyword
    auto ifFalseStatement = parseStatement(scanner_);
    if (!ifFalseStatement) {
        setError("Expected statement after else", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    return std::make_unique<IfStatement>(std::move(conditionExpression), std::move(ifTrueStatement),
                                         std::move(ifFalseStatement));
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
