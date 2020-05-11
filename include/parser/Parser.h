#ifndef TEXMINATOR_PARSER_H
#define TEXMINATOR_PARSER_H

#include <memory>

#include "Scanner.h"
#include "Environment.h"
#include "ParseError.h"

class Parser {
public:
    void parse(Scanner& scanner_);

private:
    /**
     * Initialize standard functions and variables to a local scope
     *
     * What this function does is loading a predefined functions like get() to a scope, which then can be
     * later used in code without implementing it manually.
     */
    void initializeFirstScope();
    std::unique_ptr<Statement> parseAfterKeyword(Scanner &scanner_);
    std::unique_ptr<Statement> parseAfterIdentifier(Scanner &scanner_);

    std::unique_ptr<Statement> parseVariableDeclaration(Scanner &scanner_, const ValueEnum type);

    std::unique_ptr<Expression> parseExpression(Scanner &scanner_);
    std::unique_ptr<Expression> parseVariableOrFunctionExpression(Scanner &scanner_);

    void setError(const std::string &err, unsigned int line_, unsigned int pos_);
    void printErrorMsg() const;

    Environment environment;
    std::list<ParseError> errors;
};


#endif //TEXMINATOR_PARSER_H
