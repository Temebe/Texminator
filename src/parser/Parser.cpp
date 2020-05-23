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
#include <parser/statements/ForStatement.h>
#include <parser/statements/MatchStatement.h>
#include <parser/statements/WriteStatement.h>
#include <parser/statements/ExpressionStatement.h>
#include <parser/expressions/FormattedStringExpression.h>
#include <parser/statements/IfMatchesStatement.h>
#include <parser/expressions/ReadLineExpression.h>
#include <parser/expressions/ReadCharExpression.h>
#include <parser/statements/ReturnStatement.h>
#include <parser/statements/FunctionDeclarationStatement.h>
#include "parser/Parser.h"
#include "HornerHash.h"

void Parser::parse(Scanner &scanner_) {
    initializeFirstScope();
    environment.createNewScope(local);
    std::unique_ptr<Statement> statement;

    for (Token token = scanner_.getCurrentToken();
         token.type != unknown && token.type != fileEnd;
         token = scanner_.getCurrentToken()) {

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

    while (token.type == comment) {
        token = scanner_.consume();
    }

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
                if (scanner_.peek().type == identifier) {
                    return parseIfMatchesStatement(scanner_);
                }
                return parseIfStatement(scanner_);
            }

        case constHornerHash("use"):
            if (token.value == "use") {
                return parseAliasDeclaration(scanner_);
            }

        case constHornerHash("for"):
            if (token.value == "for") {
                return parseForStatement(scanner_);
            }

        case constHornerHash("match"):
            if (token.value == "match") {
                return parseMatchStatement(scanner_);
            }

        case constHornerHash("fun"):
            if (token.value == "fun") {
                return parseFunctionDeclarationStatement(scanner_);
            }

        case constHornerHash("return"):
            if (token.value == "return") {
                return parseReturnStatement(scanner_);
            }


        default:
            break;
    }
    return std::unique_ptr<Statement>();
}

std::unique_ptr<Statement> Parser::parseAfterIdentifier(Scanner &scanner_) {
    Token token = scanner_.getCurrentToken();
    std::string identifier;

    if (token.type != TokenType::identifier) {
        return std::unique_ptr<Statement>();
    }
    identifier = token.value;
    token = scanner_.consume();

    switch (token.type) {
        case writeOperator:
            return parseWriteStatement(scanner_, identifier);

        case assignOperator:
        case addAssignOperator:
        case subAssignOperator:
        case divAssignOperator:
        case mulAssignOperator:
            return parseExpressionStatement(scanner_, identifier, token.type);

        default:
            break;
    }

    return std::unique_ptr<Statement>();
}

void Parser::initializeFirstScope() {

}


std::unique_ptr<Statement> Parser::parseBlockStatement(Scanner &scanner_) {
    Token token = scanner_.consume();
    std::list<std::unique_ptr<Statement>> statements;


    while (token.type != rightCurlyBracket && token.type != fileEnd && token.type != unknown) {
        while (token.type == comment) {
            token = scanner_.consume();
        }

        auto statement = parseStatement(scanner_);
        if (!statement) {
            setError("Could not parse statement in statement block", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }
        token = scanner_.getCurrentToken();
        statements.push_back(std::move(statement));
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
    auto expression = parseCompoundExpression(scanner_);
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

// TODO REFACTOR REFACTOR REFACTOR REFACTOR
std::unique_ptr<Statement> Parser::parseFunctionDeclarationStatement(Scanner &scanner_) {
    Token token = scanner_.consume();
    std::string name;
    ValueEnum returnType = VOID;

    if (token.type != identifier) {
        setError("Expected function name", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    name = token.value;

    token = scanner_.consume();
    if (token.type != leftRoundBracket) {
        setError("Expected left round bracket", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume();
    std::list<Parameter> arguments;
    while (token.type != rightRoundBracket && token.type != unknown && token.type != fileEnd) {
        ValueEnum type = VOID;

        if (token.value == "unsigned") {
            type = UNSIGNED_NUMBER;
            if (scanner_.peek().value == "number") {
                token = scanner_.consume();
            }
        } else if (token.value == "number") {
            type = NUMBER;
        } else if (token.value == "char") {
            type = CHAR;
        } else if (token.value == "string") {
            type = STRING;
        } else if (token.value == "float") {
            type = FLOAT;
        } else if (token.value == "bool") {
            type = BOOL;
        } else {
            setError("Unknown type", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }
        token = scanner_.consume();

        if (token.type != identifier) {
            setError("Expected identifier", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }
        arguments.emplace_back(token.value, type);
        token = scanner_.consume();
    }

    if (token.type != rightRoundBracket) {
        setError("Expected closing bracket", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    token = scanner_.consume();

    if (token.type == keyword && token.value == "ret") {
        token = scanner_.consume();

        if (token.value == "unsigned") {
            returnType = UNSIGNED_NUMBER;
            if (scanner_.peek().value == "number") {
                token = scanner_.consume();
            }
        } else if (token.value == "number") {
            returnType = NUMBER;
        } else if (token.value == "char") {
            returnType = CHAR;
        } else if (token.value == "string") {
            returnType = STRING;
        } else if (token.value == "float") {
            returnType = FLOAT;
        } else if (token.value == "bool") {
            returnType = BOOL;
        } else {
            setError("Unknown return type", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }
        token = scanner_.consume();
    }

    auto body = parseStatement(scanner_);
    if (!body) {
        setError("Could not parse function body", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    return std::make_unique<FunctionDeclarationStatement>(std::move(name), std::move(body),
                                                          std::move(arguments), returnType);
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

std::unique_ptr<Statement> Parser::parseIfMatchesStatement(Scanner &scanner_) {
    auto token = scanner_.consume();
    std::string variableName;
    std::unique_ptr<Statement> trueStatement, falseStatement;
    std::unique_ptr<Expression> stringToMatch;

    if (token.type != identifier) {
        setError("Expected identifier", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    variableName = token.value;
    token = scanner_.consume();
    if (!(token.type == keyword && token.value == "matches")) {
        setError("Expected matches keyword", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume();
    stringToMatch = parseSimpleExpression(scanner_);
    if (!stringToMatch) {
        setError("Could not parse regex expression", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    trueStatement = parseStatement(scanner_);
    if (!trueStatement) {
        setError("Could not parse if body", scanner_.getCurrentToken().line, scanner_.getCurrentToken().pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.getCurrentToken();
    if (token.type == keyword && token.value == "else") {
        falseStatement = parseStatement(scanner_);
        if (!falseStatement) {
            setError("Could not parse else body", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }
    }
    return std::make_unique<IfMatchesStatement>(std::move(trueStatement), std::move(falseStatement),
                                                std::move(variableName), std::move(stringToMatch));
}

std::unique_ptr<Statement> Parser::parseForStatement(Scanner &scanner_) {
    Token token = scanner_.consume();
    ForType type;
    std::string iteratorName, sourceName;
    std::unique_ptr<Statement> body;

    if (token.type == keyword && token.value == "char") {
        type = perCharacter;
    } else if (token.type == keyword && token.value == "line") {
        type = perLine;
    } else {
        setError(R"(Expected "char" or "line" keywords)", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume();
    if (token.type != identifier) {
        setError("Expected identifier", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    iteratorName = token.value;

    token = scanner_.consume();
    if (!(token.type == keyword && token.value == "in")) {
        setError("Expected \"in\" keyword", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume();
    if (token.type != identifier) {
        setError("Expected stream's identifier", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    sourceName = token.value;

    token = scanner_.consume();
    if (token.type != colon) {
        setError("Missing colon before for's body", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.consume();
    body = parseStatement(scanner_);
    if (!body) {
        setError("Could not parse for's body", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    return std::make_unique<ForStatement>(iteratorName, sourceName, type, std::move(body));
}

// TODO Can you shorten this?
std::unique_ptr<Statement> Parser::parseMatchStatement(Scanner &scanner_) {
    Token token = scanner_.consume();
    std::unique_ptr<Expression> matchExp;

    matchExp = parseSimpleExpression(scanner_);
    if (!matchExp) {
        setError("Could not parse match expression", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.getCurrentToken();
    if (token.type != colon) {
        setError("Expected colon", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    token = scanner_.consume(); // skip colon

    bool anyFound = false;
    std::unique_ptr<Statement> anyStatement;
    MatchList casesList;
    while (!(token.type == keyword && token.value == "matchend") && token.type != fileEnd && token.type != unknown) {
        auto caseExp = parseCompoundExpression(scanner_);
        if (!caseExp) {
            if (token.type == keyword && token.value == "any") {
                anyFound = true;
            } else {
                setError("Unable to parse case expression", token.line, token.pos);
                return std::unique_ptr<Statement>();
            }
        }

        token = scanner_.getCurrentToken();
        if (token.type != colon) {
            setError("Expected colon", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }

        token = scanner_.consume(); // skip colon
        auto caseBody = parseStatement(scanner_);
        if (!caseBody) {
            setError("Could not parse case body", token.line, token.pos);
            return std::unique_ptr<Statement>();
        }

        if (anyFound) {
            anyStatement = std::move(caseBody);
            anyFound = false;
        } else {
            casesList.push_front({std::move(caseExp), std::move(caseBody)});
        }
        token = scanner_.getCurrentToken();
    }

    token = scanner_.consume();
    if (token.type != semicolon) {
        setError("Expected semicolon", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip semicolon
    return std::make_unique<MatchStatement>(std::move(matchExp), std::move(casesList), std::move(anyStatement));
}

// TODO both functions share most of the code, maybe split it somehow
std::unique_ptr<Statement> Parser::parseWriteStatement(Scanner &scanner_, const std::string& identifier_) {
    Token token = scanner_.consume();
    std::unique_ptr<Expression> expression = parseCompoundExpression(scanner_);

    if (!expression) {
        setError("Could not parse expression properly", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    token = scanner_.getCurrentToken();
    if (token.type != semicolon) {
        setError("Missing semicolon", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip semicolon
    return std::make_unique<WriteStatement>(identifier_, std::move(expression));
}

std::unique_ptr<Statement> Parser::parseExpressionStatement(Scanner &scanner_, const std::string& identifier_,
                                                            const TokenType operation_) {
    Token token = scanner_.consume();
    std::unique_ptr<Expression> expression = parseCompoundExpression(scanner_);

    if (!expression) {
        setError("Could not parse expression properly", token.line, token.pos);
        return {};
    }

    token = scanner_.getCurrentToken();
    if (token.type != semicolon) {
        setError("Missing semicolon", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    scanner_.consume(); // skip semicolon
    return std::make_unique<ExpressionStatement>(identifier_, operation_, std::move(expression));
}

std::unique_ptr<Statement> Parser::parseReturnStatement(Scanner &scanner_) {
    Token token = scanner_.consume();

    auto exp = parseCompoundExpression(scanner_);
    token = scanner_.getCurrentToken();
    if (!exp) {
        setError("Could not parse returning expression", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }

    if (token.type != semicolon) {
        setError("Expected semicolon", token.line, token.pos);
        return std::unique_ptr<Statement>();
    }
    scanner_.consume(); // skip semicolon

    return std::make_unique<ReturnStatement>(std::move(exp));
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
        case stringLiteral:
            if (scanner_.peek().type == dot) {
                return parseFormattedStringExpression(scanner_);
            }
        case numericLiteral:
        case floatLiteral:
            leftExpression = std::make_unique<LiteralExpression>(token);
            scanner_.consume();
            break;

        case identifier:
            return parseVariableOrFunctionExpression(scanner_);

        case keyword:
            if (token.value == "true") {
                leftExpression = std::make_unique<LiteralExpression>(true);
            } else if (token.value == "false") {
                leftExpression = std::make_unique<LiteralExpression>(false);
            }
            scanner_.consume();
            break;

        case readCharLeftOperator:
        case readLineLeftOperator:
            return parseReadExpression(scanner_);

        default:
            break;

    }

    return leftExpression;
}


std::unique_ptr<Expression> Parser::parseFormattedStringExpression(Scanner &scanner_) {
    Token token = scanner_.getCurrentToken();
    std::unique_ptr<Expression> stringExp;

    if (token.type != stringLiteral) {
        setError("Expected string literal", token.line, token.pos);
        return std::unique_ptr<Expression>();
    }
    stringExp = std::make_unique<LiteralExpression>(token);

    token = scanner_.consume();
    if (token.type != dot) {
        setError("Expected dot", token.line, token.pos);
        return std::unique_ptr<Expression>();
    }

    token = scanner_.consume();
    if (token.type != leftRoundBracket) {
        setError("Expected start of arguments for formatted string", token.line, token.pos);
        return std::unique_ptr<Expression>();
    }

    token = scanner_.consume();
    std::list<std::unique_ptr<Expression>> arguments;
    while (token.type != rightRoundBracket && token.type != unknown && token.type != fileEnd) {
        std::unique_ptr<Expression> argument = parseSimpleExpression(scanner_);
        if (!argument) {
            setError("Could not parse an argument of formatted string", token.line, token.pos);
            return std::unique_ptr<Expression>();
        }
        arguments.push_back(std::move(argument));
        token = scanner_.getCurrentToken();
        if (token.type != comma) {
            break;
        }
        token = scanner_.consume();
    }

    token = scanner_.consume();
    return std::make_unique<FormattedStringExpression>(std::move(stringExp), std::move(arguments));
}

std::unique_ptr<Expression> Parser::parseVariableOrFunctionExpression(Scanner &scanner_) {
    auto token = scanner_.getCurrentToken();

    if (token.type != identifier) {
        setError("Expected identifier", token.line, token.pos);
        return {};
    }
    std::string name = token.value;
    token = scanner_.consume();

    if (token.type != leftRoundBracket) {
        return std::make_unique<VariableExpression>(name);
    }

    token = scanner_.consume(); // skip first bracket indicating parameters
    const unsigned int argLine = token.line, argPos = token.pos;
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

        if (token.type == comma) {
            token = scanner_.consume();
        }
    }

    if (scanner_.getCurrentToken().type == rightRoundBracket) {
        scanner_.consume();
        return std::make_unique<FunctionCallExpression>(name, std::move(args));
    }

    setError("Expected list of arguments ", argLine, argPos);
    return {};
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
            case nextLineOperator:
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

std::unique_ptr<Expression> Parser::parseReadExpression(Scanner &scanner_) {
    Token token = scanner_.getCurrentToken();
    if (token.type != readLineLeftOperator && token.type != readCharLeftOperator) {
        return std::unique_ptr<Expression>();
    }

    TokenType rightType = token.type == readLineLeftOperator ? readLineRightOperator : readCharRightOperator;
    token = scanner_.consume();
    auto expression = parseCompoundExpression(scanner_);
    if (!expression) {
        setError("Could not parse expression to read", token.line, token.pos);
        return std::unique_ptr<Expression>();
    }

    token = scanner_.getCurrentToken();
    if (token.type != rightType) {
        setError("Closing bracket of read expression not found", token.line, token.pos);
        return std::unique_ptr<Expression>();
    }

    scanner_.consume(); // skip closing character
    if (rightType == readLineRightOperator) {
        return std::make_unique<ReadLineExpression>(std::move(expression));
    } else {
        return std::make_unique<ReadCharExpression>(std::move(expression));
    }
}

