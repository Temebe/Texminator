#include "StringSource.h"

#include <utility>

// TODO Move code from goNext and use is in constructor

StringSource::StringSource(std::string code_)
    : code(std::move(code_)) {
    codeIt = code.begin();
    setCurrentChar(*(codeIt++));
    setNextChar(*(codeIt++));
}

void StringSource::goNext() {
    setCurrentChar(peek());

    if (getChar() == '\n') {
        setLineNumber(getLineNumber() + 1);
        setCharPos(0);
    } else {
        setCharPos(getCharPos() + 1);
    }

    if (codeIt == code.end()) {
        setNextChar(std::char_traits<char>::eof());
    } else {
        setNextChar(*(codeIt++));
    }
}