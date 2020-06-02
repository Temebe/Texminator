#ifndef TEXMINATOR_PARSER_H
#define TEXMINATOR_PARSER_H

#include <string>
#include <memory>
#include <set>

#include "parser/statements/Statement.h"
#include "parser/expressions/Expression.h"
#include "Scanner.h"
#include "Environment.h"
#include "ParseError.h"

class Parser {
public:
    void parse(Scanner& scanner_, const std::vector<std::string> &args_ = {});
    std::unique_ptr<Statement> parseStatement(Scanner &scanner_);
    std::unique_ptr<Expression> parseCompoundExpression(Scanner &scanner_,  bool stopOnRoundBracket_ = false);

private:

    // TODO Lookup table?
    std::unique_ptr<Statement> parseAfterKeyword(Scanner &scanner_);
    std::unique_ptr<Statement> parseAfterIdentifier(Scanner &scanner_);

    std::unique_ptr<Statement> parseBlockStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseVariableDeclaration(Scanner &scanner_, ValueEnum type = VOID);
    std::unique_ptr<Statement> parseFunctionDeclarationStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseAliasDeclaration(Scanner &scanner_);
    std::unique_ptr<Statement> parseOpenStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseIfStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseIfMatchesStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseForStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseMatchStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseWriteStatement(Scanner &scanner_, const std::string& identifier);
    std::unique_ptr<Statement> parseIdentifierExpressionStatement(Scanner &scanner_, const std::string& identifier_,
                                                                  const TokenType operation_);
    std::unique_ptr<Statement> parseIncrementExpressionStatement(Scanner &scanner_, const std::string& identifier_);
    std::unique_ptr<Statement> parseReturnStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseBreakStatement(Scanner &scanner_);
    std::unique_ptr<Statement> parseContinueStatement(Scanner &scanner_);


    std::unique_ptr<Expression> parseSimpleExpression(Scanner &scanner_);
    std::unique_ptr<Expression> parseFormattedStringExpression(Scanner &scanner_);
    std::unique_ptr<Expression> parseReadExpression(Scanner &scanner_);
    std::unique_ptr<Expression> parseVariableOrFunctionExpression(Scanner &scanner_, const std::string &identifier_ = "");

    bool consumeMatching(Scanner &scanner_, std::list<TokenType> tokens_);
    bool checkAhead(Scanner &scanner_, std::list<TokenType> tokens_, bool includeFirst_ = true);

    void setError(const std::string &err, unsigned int line_, unsigned int pos_);
    void printErrorMsg() const;

    std::list<ParseError> errors;
};


#endif //TEXMINATOR_PARSER_H
