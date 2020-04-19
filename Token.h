//
// Created by tromba on 18.04.2020.
//

#ifndef TEXMINATOR_TOKEN_H
#define TEXMINATOR_TOKEN_H

#include <string>
#include <map>

// TODO square bracket as read operator or as punctuator?
enum TokenType {
    identifier, /** Identifier of variable */
    keyword, /** if, while, else, ... */

    /** Literals */
    numericLiteral, /** Number with sign or not */
    stringLiteral,  /** Any string in brackets  */

    /** Arithmetic operators */
    addOperator, /** Addition       + */
    subOperator, /** Subtraction    - */
    mulOperator, /** Multiplication * */
    divOperator, /** Division       / */
    modOperator, /** Modulus        % */

    /** Comparison operators */
    eqOperator, /** equal            == */
    neOperator, /** not equal        != */
    ltOperator, /** less             <  */
    leOperator, /** less or equal    <= */
    gtOperator, /** greater          >  */
    geOperator, /** greater or equal >= */

    /** Assignment operators */
    assignOperator,    /** simple assignment         =  */
    addAssignOperator, /** addition assignment       += */
    subAssignOperator, /** subtraction assignment    -= */
    divAssignOperator, /** multiplication assignment *= */
    mulAssignOperator, /** division assignment       /= */

    /** Logical operators */
    andOperator, /** AND operator && */
    orOperator,  /** OR operator  || */
    notOperator, /** NOT operator !  */

    /** File operators */
    writeOperator,         /**  ->  */
    readLineLeftOperator,  /**  [[  */
    readLineRightOperator, /**  ]]  */
    readCharLeftOperator,  /**  ]   */
    readCharRightOperator, /**  ]   */
    nextCharOperator,      /**  +   */
    nextLineOperator,      /**  ++  */

    /** Punctuators */
    leftCurlyBracket,  /**  {  */
    rightCurlyBracket, /**  }  */
    leftRoundBracket,  /**  (  */
    rightRoundBracket, /**  (  */
    colon,             /**  :  */
    semicolon,         /**  ;  */
    comma,             /**  ,  */

    /** Miscellaneous */
    fileEnd,    /** Not really a token, just an information that file has ended and there is no "next token" */
    unknown     /** Scanner received token which he does not understand */

};

// TODO Maybe make Token class, not a struct?
struct Token {

    std::string value;
    TokenType type = unknown;
    unsigned int line = 0; /** Line in which token is */
    unsigned int pos = 0;  /** Position in line */

    static const std::map<TokenType, const char*> typeMap;

    [[nodiscard]] const char* toString() const {
        return typeMap.at(type);
    };

    static const char* toString(const TokenType type_) {
        return typeMap.at(type_);
    }
};

const std::map<TokenType, const char*> Token::typeMap = {
        { identifier, "identifier" },
        { keyword, "keyword" },

        { numericLiteral, "numeric literal" },
        {stringLiteral,     "string literal" },

        {addOperator,       "addition operator" },
        {subOperator,       "subtraction operator" },
        {mulOperator,       "multiplication operator" },
        {divOperator,       "division operator" },
        {modOperator,       "modulus operator " },

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
        { orOperator, "logical OR operator" },
        { notOperator, "logical NOT operator" },

        { writeOperator, "write operator (->)" },
        { readLineLeftOperator, "left \"read line\" operator [[" },
        { readLineRightOperator, "right \"read line\" operator ]]" },
        { readCharLeftOperator, "left \"read character\" operator [" },
        { readCharRightOperator, "right \"read character\" operator ]" },
        { nextCharOperator, "next character operator +" },
        { nextLineOperator, "next line operator ++" },

        { leftCurlyBracket, "left curly bracket {" },
        { rightCurlyBracket, "right curly bracket }" },
        { leftRoundBracket, "left round bracket (" },
        { rightRoundBracket, "right round bracket )" },
        { colon, "colon" },
        { semicolon, "semicolon" },
        { comma, "comma" },

        { fileEnd, "end of file" },
        { unknown, "unknown" }
};

#endif //TEXMINATOR_TOKEN_H
