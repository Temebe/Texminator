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
    //addOperator, /** Addition       + */
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
    //nextCharOperator,      /**  +   */
    nextLineOperator,      /**  ++  */

    /** Punctuators */
    leftCurlyBracket,  /**  {  */
    rightCurlyBracket, /**  }  */
    leftRoundBracket,  /**  (  */
    rightRoundBracket, /**  (  */
    colon,             /**  :  */
    semicolon,         /**  ;  */
    dot,               /**  .  */
    comma,             /**  ,  */
    plus,              /**  +  */

    /** Miscellaneous */
    comment,    /** Any text after symbol //, tokenized so may be later used for i. e. automatic code generation */
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

#endif //TEXMINATOR_TOKEN_H
