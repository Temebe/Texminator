#include "../include/Source.h"


char Source::getChar() {
    return currentChar;
}

char Source::peek() {
    return nextChar;
}

unsigned int Source::getLineNumber() const {
    return Source::lineNumber;
}

unsigned int Source::getCharPos() const {
    return Source::charPos;
}

void Source::setCharPos(const unsigned int charPos_) {
    Source::charPos = charPos_;
}

void Source::setLineNumber(const unsigned int lineNumber_) {
    Source::lineNumber = lineNumber_;
}

void Source::setCurrentChar(const char newChar_) {
    currentChar = newChar_;
}

void Source::setNextChar(const char newChar_) {
    nextChar = newChar_;
}
