#include <unordered_set>
#include "../include/Scanner.h"

// TODO Maximum length of a token

Scanner::Scanner(std::unique_ptr<Source> source) {
    this->source = std::move(source);

    createTokenFunctions.emplace_back(&Scanner::createPunctuatorToken);
    createTokenFunctions.emplace_back(&Scanner::createArithmeticOperatorToken);
    createTokenFunctions.emplace_back(&Scanner::createComparisonOperatorToken);
    createTokenFunctions.emplace_back(&Scanner::createAssignmentOperatorToken);
    createTokenFunctions.emplace_back(&Scanner::createLogicalOperatorToken);
    createTokenFunctions.emplace_back(&Scanner::createFileOperatorToken);
    createTokenFunctions.emplace_back(&Scanner::createNumericOrFloatLiteralToken);
    createTokenFunctions.emplace_back(&Scanner::createStringLiteralToken);
    createTokenFunctions.emplace_back(&Scanner::createIdentifierOrKeywordToken);
    createTokenFunctions.emplace_back(&Scanner::createCommentToken);

    // create two times token which will result in creating currentToken and nextToken for the first time
    createToken();
    createToken();
    tokenIterator = tokens.begin();
}

void Scanner::createToken() {
    Token newToken;
    do {
        newToken = generateNewToken();
    } while (newToken.type == comment);
    tokens.emplace_back(newToken);
}

const Token &Scanner::consume(const int amount_) {
    if (amount_ <= 0) {
        return *tokenIterator;
    }

    auto firstIt = tokenIterator;
    std::advance(firstIt, -(amount_ - 1));
    tokens.erase(firstIt, tokenIterator++);

    if (tokenIterator == tokens.end()) {
        createToken();
        tokenIterator = --tokens.end();
    }

    return *tokenIterator;
}

const Token &Scanner::getCurrentToken() const {
    return *tokenIterator;
}

const Token &Scanner::getNextToken() {
    if (++tokenIterator == tokens.end()) {
        createToken();
        tokenIterator = --tokens.end();
    }

    return *tokenIterator;
}

Token Scanner::peek() {
    auto result = getNextToken();
    goBack();
    return result;
}

void Scanner::goBack(const int amount_) {
    if (amount_ < 0) {
        return goForward(-amount_);
    }

    std::advance(tokenIterator, -amount_);
}

void Scanner::goForward(const int amount_) {
    if (amount_ < 0) {
        return goBack(-amount_);
    }

    for (auto i = amount_; i > 0; --i) {
        getNextToken();
    }
}

Token Scanner::generateNewToken() {
    std::optional<Token> newToken;
    char currentChar = source->getChar();

    // First get rid of white spaces
    while (isspace(currentChar) && currentChar != std::char_traits<char>::eof()) {
        source->goNext();
        currentChar = source->getChar();
    }

    unsigned int line = source->getLineNumber();
    unsigned int pos = source->getCharPos();

    if (currentChar == std::char_traits<char>::eof()) {
        newToken.emplace();
        newToken->type = TokenType::fileEnd;
        return newToken.value();
    }

    // Do every create token function until one of them succeed
    for (const auto& function : createTokenFunctions) {
        newToken = function(*this);
        if (newToken) {
            source->goNext();
            break;
        }
    }

    if (!newToken) { // no function could assign a valid token
        newToken.emplace(); // create default token which is of unknown type
    }
    newToken->pos = pos;
    newToken->line = line;

    return newToken.value();
}

// TODO newline as variable or keyword?
bool Scanner::isKeyword(const std::string &word_) {
    static const std::unordered_set<std::string> keywordSet = {
            "any",   "as",       "bool",     "char",
            "close", "line",     "else",     "false",
            "float", "for",      "fun",      "if",
            "in",    "match",    "matchend", "matches",
            "none",  "number",   "open",     "read",
            "ret",   "return",   "string",   "to",
            "true",  "unsigned", "use",      "void",
            "while", "write"
    };

    return keywordSet.find(word_) != keywordSet.end();
}

std::optional<Token> Scanner::createPunctuatorToken() {
    Token token;
    token.value = source->getChar();

    switch (source->getChar()) {
        case '{':
            token.type = leftCurlyBracket;
            return std::optional<Token>(token);

        case '}':
            token.type = rightCurlyBracket;
            return std::optional<Token>(token);

        case '(':
            token.type = leftRoundBracket;
            return std::optional<Token>(token);

        case ')':
            token.type = rightRoundBracket;
            return std::optional<Token>(token);

        case ':':
            token.type = colon;
            return std::optional<Token>(token);

        case ';':
            token.type = semicolon;
            return std::optional<Token>(token);

        case ',':
            token.type = comma;
            return std::optional<Token>(token);

        case '.':
            token.type = dot;
            return std::optional<Token>(token);

        case '+':
            if (source->peek() == '+' || source->peek() == '=') {
                return std::optional<Token>();
            }
            token.type = plus;
            return std::optional<Token>(token);

        default:
            return std::optional<Token>();
    }
}

std::optional<Token> Scanner::createArithmeticOperatorToken() {
    Token token;
    token.value = source->getChar();

    switch (source->getChar()) {
        case '-':
            if (source->peek() == '>' || source->peek() == '=') {
                return std::optional<Token>();
            }
            token.type = subOperator;
            return std::optional<Token>(token);

        case '*':
            if (source->peek() == '=') {
                return std::optional<Token>();
            }
            token.type = mulOperator;
            return std::optional<Token>(token);

        case '/':
            if (source->peek() == '/' || source->peek() == '=') {
                return std::optional<Token>();
            }
            token.type = divOperator;
            return std::optional<Token>(token);

        case '%':
            token.type = modOperator;
            return std::optional<Token>(token);

        default:
            return std::optional<Token>();
    }
}

std::optional<Token> Scanner::createComparisonOperatorToken() {
    Token token;
    token.value = source->getChar();

    switch (source->getChar()) {
        case '=':
            if (source->peek() == '=') { // ==
                token.value += source->peek();
                token.type = eqOperator;
                source->goNext();
                return std::optional<Token>(token);
            }
            return std::optional<Token>();

        case '!':
            if (source->peek() == '=') { // !=
                token.value += source->peek();
                token.type = neOperator;
                source->goNext();
                return std::optional<Token>(token);
            }
            return std::optional<Token>();

        case '<':
            if (source->peek() == '=') { // <=
                token.value += source->peek();
                token.type = leOperator;
                source->goNext();
            } else {
                token.type = ltOperator;
            }
            return std::optional<Token>(token);

        case '>':
            if (source->peek() == '=') { // >=
                token.value += source->peek();
                token.type = geOperator;
                source->goNext();
            } else {
                token.type = gtOperator;
            }
            return std::optional<Token>(token);

        default:
            return std::optional<Token>();
    }
}

std::optional<Token> Scanner::createAssignmentOperatorToken() {
    Token token;
    token.value = source->getChar();

    if (source->getChar() == '=' && source->peek() != '=') {
        token.type = assignOperator;
        return std::optional<Token>(token);
    }

    if (source->peek() != '=') {
        return std::optional<Token>();
    }

    switch (source->getChar()) {
        case '+':
            token.type = addAssignOperator;
            break;

        case '-':
            token.type = subAssignOperator;
            break;

        case '*':
            token.type = mulAssignOperator;
            break;

        case '/':
            token.type = divAssignOperator;
            break;
    }

    if (token.type != TokenType::unknown) {
        token.value += source->peek();
        source->goNext();
        return std::optional<Token>(token);
    }
    return std::optional<Token>();
}

// TODO Get rid of copy paste code
std::optional<Token> Scanner::createLogicalOperatorToken() {
    Token token;
    token.value = source->getChar();

    if (source->getChar() == '&' && source->peek() == '&') { // AND operator
        token.type = andOperator;
        token.value += source->peek();
        source->goNext();
        return std::optional<Token>(token);
    }

    else if (source->getChar() == '|' && source->peek() == '|') { // OR operator
        token.type = orOperator;
        token.value += source->peek();
        source->goNext();
        return std::optional<Token>(token);
    }

    else if (source->getChar() == '!' && source->peek() != '=') { // NOT operator
        token.type = notOperator;
        token.value += source->peek();
        source->goNext();
        return std::optional<Token>(token);
    }

    return std::optional<Token>();
}

std::optional<Token> Scanner::createFileOperatorToken() {
    Token token;
    token.value = source->getChar();

    switch (source->getChar()) {
        case '-':
            if (source->peek() == '>') { // ->
                token.type = writeOperator;
                token.value += source->peek();
                source->goNext();
                return std::optional<Token>(token);
            }
            return std::optional<Token>();

        case '+':
            if (source->peek() == '+') { // ++
                token.type = nextLineOperator;
                token.value += source->peek();
                source->goNext();
                return std::optional<Token>(token);
            }
            return std::optional<Token>();

        case '[':
            if (source->peek() == '[') { // [[
                token.type = readLineLeftOperator;
                token.value += source->peek();
                source->goNext();
            } else {
                token.type = readCharLeftOperator;
            }
            return std::optional<Token>(token);

        case ']':
            if (source->peek() == ']') { // ]]
                token.value += source->peek();
                token.type = readLineRightOperator;
                source->goNext();
            } else {
                token.type = readCharRightOperator;
            }
            return std::optional<Token>(token);

        default:
            return std::optional<Token>();
    }
}

std::optional<Token> Scanner::createNumericOrFloatLiteralToken() {
    Token token;
    token.value = source->getChar();

    if (!isdigit(source->getChar()) && source->getChar() != '+' && source->getChar() != '-') {
        return std::optional<Token>();
    }
    if (source->getChar() == '0' && isdigit(source->peek()) ) {
        return std::optional<Token>();
    }
    if ((source->getChar() == '+' || source->getChar() == '-') && isdigit(source->peek())) {
        token.value += source->peek();
        source->goNext();
    }

    token.type = numericLiteral;
    while (isdigit(source->peek()) || source->peek() == '.') {
        if (source->peek() == '.') {
            token.type = floatLiteral;
        }
        token.value += source->peek();
        source->goNext();
    }

    return std::optional<Token>(token);
}

std::optional<Token> Scanner::createStringLiteralToken() {
    Token token;

    if (source->getChar() != '\"') {
        return std::optional<Token>();
    }
    token.type = stringLiteral;

    // collect characters to string to the final " (but continue if it's escaped \")
    while (source->peek() != '\"' || source->getChar() == '\\') {
        // if closing character was not found but eof or new line instead, then token is incorrect
        if (source->peek() == '\n' || source->peek() == std::char_traits<char>::eof()) {
            token.type = unknown;
            return std::optional<Token>(token);
        }

        token.value += source->peek();
        source->goNext();
    }

    source->goNext();
    return std::optional<Token>(token);
}

std::optional<Token> Scanner::createIdentifierOrKeywordToken() {
    Token token;
    token.value = source->getChar();

    if (!isalpha(source->getChar()) && source->getChar() != '_') {
        return std::optional<Token>();
    }

    while (isalnum(source->peek()) || source->peek() == '_' || source->peek() == '.') {
        token.value += source->peek();
        source->goNext();
    }

    if (isKeyword(token.value)) {
        token.type = keyword;
    } else {
        token.type = identifier;
    }

    return std::optional<Token>(token);
}

std::optional<Token> Scanner::createCommentToken() {
    Token token;
    token.value = source->getChar();

    if (source->getChar() != '/' || source->peek() != '/') {
        return std::optional<Token>();
    }
    token.type = comment;

    // move source to the next line or to end of file
    unsigned int currentLine = source->getLineNumber();
    do {
        source->goNext();
    } while (source->getLineNumber() == currentLine && source->getChar() != std::char_traits<char>::eof());

    return std::optional<Token>(token);
}
