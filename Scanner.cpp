#include <unordered_set>
#include "Scanner.h"

Scanner::Scanner(std::unique_ptr<Source> source) {
    this->source = std::move(source);
}

Token Scanner::nextToken() {
    return Token();
}

// TODO newline as variable or keyword?
bool Scanner::isKeyword(const std::string &word_) {
    static const std::unordered_set<std::string> keywordSet = {
            "any",    "as",    "bool",     "char",
            "close",  "line",  "else",     "false",
            "float",  "for",   "fun",      "if",
            "in",     "match", "matchend", "none",
            "number", "open",  "ret",      "return",
            "string", "to",    "true",     "unsigned",
            "void",   "while"
    };

    return keywordSet.find(word_) != keywordSet.end();
}
