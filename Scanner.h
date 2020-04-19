#ifndef TEXMINATOR_SCANNER_H
#define TEXMINATOR_SCANNER_H

#include <memory>

#include "Source.h"
#include "Token.h"

class Scanner {
public:
    Scanner(std::unique_ptr<Source> source_);
    Token nextToken();

private:
    static bool isKeyword(const std::string &word_);
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
};


#endif //TEXMINATOR_SCANNER_H
