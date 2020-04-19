/**
 * @file Scanner.h
 *
 * @brief Scanner converts input into basic tokens
 *
 * Scanner uses source (which may be a file or stdin, depending on implementation), reads from it and from given
 * characters creates tokens. Tokens are made only when nextToken() method is used which also means that Source object
 * stays the same until someone uses this method.
 */

#ifndef TEXMINATOR_SCANNER_H
#define TEXMINATOR_SCANNER_H

#include <memory>
#include <functional>
#include <list>

#include "Source.h"
#include "Token.h"

class Scanner {
public:
    Scanner(std::unique_ptr<Source> source_);
    Token nextToken();

private:
    static bool isKeyword(const std::string &word_);

    /**
     * checkIf functions
     *
     * In general these functions work the same: they check source for a token. If there is a token they
     * modify given token object to reflect source and return true, otherwise they do not modify token nor source.
     *
     * @return true if token is of given type
     */

    bool checkIfPunctuator(Token &token_);
    bool checkIfArithmeticOperator(Token &token_);
    bool checkIfComparisonOperator(Token &token_);
    bool checkIfAssignmentOperator(Token &token_);
    bool checkIfLogicalOperator(Token &token_);
    bool checkIfFileOperator(Token &token_);
    bool checkIfNumericLiteral(Token &token_);
    bool checkIfStringLiteral(Token &token_);
    bool checkIfIdentifierOrKeyword(Token &token_);
    bool checkIfComment(Token &token_);

    std::unique_ptr<Source> source;
    std::list<std::function<bool(Scanner&, Token&)>> checkFunctions;
};


#endif //TEXMINATOR_SCANNER_H
