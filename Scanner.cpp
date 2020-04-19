#include <unordered_set>
#include "Scanner.h"

// TODO Maximum length of a token

Scanner::Scanner(std::unique_ptr<Source> source) {
    this->source = std::move(source);
}

Token Scanner::nextToken() {
    Token newToken;
    char currentChar = source->getChar();

    // First get rid of white spaces
    while (isspace(currentChar) && currentChar != std::char_traits<char>::eof()) {
        source->goNext();
        currentChar = source->getChar();
    }

    newToken.line = source->getLineNumber();
    newToken.pos = source->getCharPos();
    newToken.value = currentChar;

    if (currentChar == std::char_traits<char>::eof()) { // TODO use goNext or not? Move to function or not?
        newToken.type = TokenType::fileEnd;
        return newToken;
    }

    if (checkIfPunctuator(newToken)
        || checkIfArithmeticOperator(newToken)
        || checkIfComparisonOperator(newToken)
        || checkIfAssignmentOperator(newToken)
        || checkIfLogicalOperator(newToken)
        || checkIfFileOperator(newToken)
        || checkIfNumericLiteral(newToken)
        || checkIfStringLiteral(newToken)
        || checkIfIdentifierOrKeyword(newToken)
        || checkIfComment(newToken)) {
        source->goNext(); // move to character that does not belong to this token
        return newToken;
    }


    return Token();
}

// TODO newline as variable or keyword?
bool Scanner::isKeyword(const std::string &word_) {
    static const std::unordered_set<std::string> keywordSet = {
            "any",      "as",     "bool",     "char",
            "close",    "line",   "else",     "false",
            "float",    "for",    "fun",      "if",
            "in",       "match",  "matchend", "none",
            "number",   "open",   "read",     "ret",
            "return",   "string", "to",       "true",
            "unsigned", "use",    "void",     "while",
            "write"
    };

    return keywordSet.find(word_) != keywordSet.end();
}

bool Scanner::checkIfPunctuator(Token &token_) {
    switch (source->getChar()) {
        case '{':
            token_.type = leftCurlyBracket;
            return true;

        case '}':
            token_.type = rightCurlyBracket;
            return true;

        case '(':
            token_.type = leftRoundBracket;
            return true;

        case ')':
            token_.type = rightRoundBracket;
            return true;

        case ':':
            token_.type = colon;
            return true;

        case ';':
            token_.type = semicolon;
            return true;

        case ',':
            token_.type = comma;
            return true;

        case '.':
            token_.type = dot;
            return true;

        case '+':
            if (source->peek() == '+' || source->peek() == '=') {
                return false;
            }
            token_.type = plus;
            return true;

        default:
            return false;
    }
}

bool Scanner::checkIfArithmeticOperator(Token &token_) {
    switch (source->getChar()) {
//        case '+':
//            token_.type = addOperator;
//            return true;

        case '-':
            if (source->peek() == '>' || source->peek() == '=') {
                return false;
            }
            token_.type = subOperator;
            return true;

        case '*':
            if (source->peek() == '=') {
                return false;
            }
            token_.type = mulOperator;
            return true;

        case '/':
            if (source->peek() == '/' || source->peek() == '=') {
                return false;
            }
            token_.type = divOperator;
            return true;

        case '%':
            token_.type = modOperator;
            return true;

        default:
            return false;
    }
}

bool Scanner::checkIfComparisonOperator(Token &token_) {
    switch (source->getChar()) {
        case '=':
            if (source->peek() == '=') { // ==
                token_.value += source->peek();
                token_.type = eqOperator;
                source->goNext();
                return true;
            }
            return false;

        case '!':
            if (source->peek() == '=') { // !=
                token_.value += source->peek();
                token_.type = neOperator;
                source->goNext();
                return true;
            }
            return false;

        case '<':
            if (source->peek() == '=') { // <=
                token_.value += source->peek();
                token_.type = leOperator;
                source->goNext();
            } else {
                token_.type = ltOperator;
            }
            return true;

        case '>':
            if (source->peek() == '=') { // >=
                token_.value += source->peek();
                token_.type = geOperator;
                source->goNext();
            } else {
                token_.type = gtOperator;
            }
            return true;

        default:
            return false;
    }
}

bool Scanner::checkIfAssignmentOperator(Token &token_) {
    if (source->getChar() == '=' && source->peek() != '=') {
        token_.type = assignOperator;
        return true;
    }

    if (source->peek() != '=') {
        return false;
    }

    switch (source->getChar()) {
        case '+':
            token_.type = addAssignOperator;
            break;

        case '-':
            token_.type = subAssignOperator;
            break;

        case '*':
            token_.type = mulAssignOperator;
            break;

        case '/':
            token_.type = divAssignOperator;
            break;
    }

    if (token_.type != TokenType::unknown) {
        token_.value += source->peek();
        source->goNext();
        return true;
    }
    return false;
}

// TODO Get rid of copy paste code
bool Scanner::checkIfLogicalOperator(Token &token_) {
    if (source->getChar() == '&' && source->peek() == '&') { // AND operator
        token_.type = andOperator;
        token_.value += source->peek();
        source->goNext();
        return true;
    }

    else if (source->getChar() == '|' && source->peek() == '|') { // OR operator
        token_.type = orOperator;
        token_.value += source->peek();
        source->goNext();
        return true;
    }

    else if (source->getChar() == '!' && source->peek() != '=') { // NOT operator
        token_.type = notOperator;
        token_.value += source->peek();
        source->goNext();
        return true;
    }

    return false;
}

bool Scanner::checkIfFileOperator(Token &token_) {
    switch (source->getChar()) {
        case '-':
            if (source->peek() == '>') { // ->
                token_.type = writeOperator;
                token_.value += source->peek();
                source->goNext();
                return true;
            }
            return false;

        case '+':
            if (source->peek() == '+') { // ++
                token_.type = nextLineOperator;
                token_.value += source->peek();
                source->goNext();
                return true;
            }
            return false;

        case '[':
            if (source->peek() == '[') { // [[
                token_.type = readLineLeftOperator;
                token_.value += source->peek();
                source->goNext();
            } else {
                token_.type = readCharLeftOperator;
            }
            return true;

        case ']':
            if (source->peek() == ']') { // ]]
                token_.value += source->peek();
                token_.type = readLineRightOperator;
                source->goNext();
            } else {
                token_.type = readLineRightOperator;
            }
            return true;

        default:
            return false;
    }
}

bool Scanner::checkIfNumericLiteral(Token &token_) {
    if (!isdigit(source->getChar())) {
        return false;
    }
    if (source->getChar() == '0' && isdigit(source->peek()) ) {
        return false;
    }

    token_.type = numericLiteral;
    while (isdigit(source->peek())) {
        token_.value += source->peek();
        source->goNext();
    }

    return true;
}

bool Scanner::checkIfStringLiteral(Token &token_) {
    if (source->getChar() != '\"') {
        return false;
    }
    token_.type = stringLiteral;

    // collect characters to string to the final " (but continue if it's escaped \")
    while (source->peek() != '\"' || source->getChar() == '\\') {
        token_.value += source->peek();
        source->goNext();
    }

    // Since we stopped right before " character, add this one too before returning
    token_.value += source->peek();
    source->goNext();
    return true;
}

bool Scanner::checkIfIdentifierOrKeyword(Token &token_) {
    if (!isalpha(source->getChar()) && source->getChar() != '_') {
        return false;
    }

    while (isalnum(source->peek()) || source->peek() == '_' || source->peek() == '.') {
        token_.value += source->peek();
        source->goNext();
    }

    if (isKeyword(token_.value)) {
        token_.type = keyword;
    } else {
        token_.type = identifier;
    }

    return true;
}

bool Scanner::checkIfComment(Token &token_) {
    if (source->getChar() != '/' || source->peek() != '/') {
        return false;
    }
    token_.type = comment;

    // move source to the next line or to end of file
    unsigned int currentLine = source->getLineNumber();
    do {
        source->goNext();
    } while (source->getLineNumber() == currentLine && source->getChar() != std::char_traits<char>::eof());

    return true;
}
