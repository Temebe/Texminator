#include <parser/statements/Statement.h>
#include <iostream>
#include <parser/statements/VariableDeclarationStatement.h>
#include "parser/Parser.h"
#include "HornerHash.h"

void Parser::parse(Scanner &scanner_) {
    initializeFirstScope();
    environment.createNewScope(local);
    std::unique_ptr<Statement> statement;

    for (Token token = scanner_.nextToken();
         token.type != unknown && token.type != fileEnd;
         token = scanner_.nextToken()) {

        switch (token.type) {
            case identifier:
                statement = parseAfterIdentifier(scanner_, token);
                break;

            case keyword:
                statement = parseAfterKeyword(scanner_, token);
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
std::unique_ptr<Statement> Parser::parseAfterKeyword(Scanner &scanner_, const Token &token_) {
    switch (hornerHash(token_.value.c_str())) {
        case constHornerHash("bool"):
            if (token_.value == "bool") {
                parseVariableDeclaration(scanner_, ValueEnum::BOOL);
            }
            break;

        case constHornerHash("char"):
            if (token_.value == "char") {
                parseVariableDeclaration(scanner_, ValueEnum::CHAR);
            }
            break;

        case constHornerHash("float"):
            if (token_.value == "float") {
                parseVariableDeclaration(scanner_, ValueEnum::FLOAT);
            }
            break;

        case constHornerHash("unsigned"):
            if (token_.value == "unsigned") {
                parseVariableDeclaration(scanner_, ValueEnum::UNSIGNED_NUMBER);
            }
            break;

        case constHornerHash("number"):
            if (token_.value == "number") {
                parseVariableDeclaration(scanner_, ValueEnum::NUMBER);
            }
            break;

        case constHornerHash("string"):
            if (token_.value == "string") {
                parseVariableDeclaration(scanner_, ValueEnum::BOOL);
            }
            break;


        default:
            break;
    }
}

std::unique_ptr<Statement> Parser::parseAfterIdentifier(Scanner &scanner_, const Token &token_) {

}

void Parser::initializeFirstScope() {

}

std::unique_ptr<Statement> Parser::parseVariableDeclaration(Scanner &scanner_, ValueEnum type) {
    Token token = scanner_.nextToken();
    // If there is word "number" after "unsigned" just skip it
    if (type == UNSIGNED_NUMBER && token.type == keyword && token.value == "number") {
        token = scanner_.nextToken();
    }
    if (token.type != identifier) {
        setError("Expected identifier", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    auto statement = std::make_unique<VariableDeclarationStatement>(token.value, type);
    token = scanner_.nextToken();
    if (token.type == semicolon) {
        return statement;
    }

    if (token.type != assignOperator) {
        setError("Expected assignment operator", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.nextToken();
    auto expression = parseExpression(scanner_, token);
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

std::unique_ptr<Expression> Parser::parseExpression(Scanner &scanner_, const Token& firstToken_) {
    switch (firstToken_.type) {
        case leftRoundBracket:
            auto expression = parseExpression(scanner_, scanner_.nextToken());

    }

    return std::unique_ptr<Expression>();
}
