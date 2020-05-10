#include "../include/Token.h"

const std::map<TokenType, const char*> Token::typeMap = {
        { identifier, "identifier" },
        { keyword, "keyword" },

        { numericLiteral, "numeric literal" },
        { stringLiteral,  "string literal" },
        { floatLiteral,   "float literal" },

        //{addOperator,      "addition operator" },
        {subOperator,       "subtraction operator" },
        {mulOperator,       "multiplication operator" },
        {divOperator,       "division operator" },
        {modOperator,       "modulus operator" },

        {eqOperator,        "comparision (equal) operator" },
        {neOperator,        "comparision (not equal) operator" },
        {ltOperator,        "comparision (less than) operator" },
        {leOperator,        "comparision (less or equal than) operator" },
        {gtOperator,        "comparision (greater than) operator" },
        {geOperator,        "comparision (greater or equal than) operator" },

        {assignOperator,    "assignment operator" },
        {addAssignOperator, "addition assignment operator" },
        {subAssignOperator, "subtraction assignment operator" },
        {divAssignOperator, "multiplication assignment operator" },
        {mulAssignOperator, "division assignment operator" },

        { andOperator, "logical AND operator" },
        { orOperator,  "logical OR operator" },
        { notOperator, "logical NOT operator" },

        { writeOperator, "write operator (->)" },
        { readLineLeftOperator, "left \"read line\" operator" },
        { readLineRightOperator, "right \"read line\" operator" },
        { readCharLeftOperator, "left \"read character\" operator" },
        { readCharRightOperator, "right \"read character\" operator" },
        //{ nextCharOperator, "next character operator +" },
        { nextLineOperator, "next line operator ++" },

        { leftCurlyBracket, "left curly bracket" },
        { rightCurlyBracket, "right curly bracket" },
        { leftRoundBracket, "left round bracket" },
        { rightRoundBracket, "right round bracket" },
        { colon, "colon" },
        { semicolon, "semicolon" },
        { dot, "dot" },
        { comma, "comma" },
        { plus, "plus" },

        { comment, "comment" },
        { fileEnd, "end of file" },
        { unknown, "unknown" }
};
