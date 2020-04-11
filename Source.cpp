#include "Source.h"

unsigned int Source::getLineNumber() {
    return Source::lineNumber;
}

unsigned int Source::getCharPos() {
    return Source::charPos;
}

void Source::setCharPos(unsigned int charPos) {
    Source::charPos = charPos;
}

void Source::setLineNumber(unsigned int lineNumber) {
    Source::lineNumber = lineNumber;
}
