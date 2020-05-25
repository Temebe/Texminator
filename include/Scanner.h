/**
 * @file Scanner.h
 *
 * @brief Scanner converts input into basic tokens
 *
 * Scanner uses source (which may be a file or stdin, depending on implementation), reads from it and from given
 * characters creates tokens. Tokens are made only when generateNewToken() method is used which also means that Source object
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
    void createToken();
    const Token& consume(const int amount_ = 1);
    const Token& getCurrentToken() const;
    const Token& getNextToken();
    Token peek();
    void goBack(const int amount_ = 1);
    void goForward(const int amount_ = 1);


private:
    Token generateNewToken();
    static bool isKeyword(const std::string &word_);

    /**
     * create token functions
     *
     * In general these functions work the same: they check source for a token. If there is a token they
     * create it as object and return it, otherwise they do not modify source. However be aware that
     * it DOES NOT set an position of token.
     *
     * @return token if source is on valid token, otherwise empty optional
     */

    std::optional<Token> createPunctuatorToken();
    std::optional<Token> createArithmeticOperatorToken();
    std::optional<Token> createComparisonOperatorToken();
    std::optional<Token> createAssignmentOperatorToken();
    std::optional<Token> createLogicalOperatorToken();
    std::optional<Token> createFileOperatorToken();
    std::optional<Token> createNumericOrFloatLiteralToken();
    std::optional<Token> createStringLiteralToken();
    std::optional<Token> createIdentifierOrKeywordToken();
    std::optional<Token> createCommentToken();

    std::unique_ptr<Source> source;
    std::list<std::function<std::optional<Token>(Scanner&)>> createTokenFunctions;
    std::list<Token> tokens;
    std::list<Token>::iterator tokenIterator;
    Token currentToken;
    Token nextToken;
};


#endif //TEXMINATOR_SCANNER_H
