#include <iostream>
#include <string>

#include "../include/FileSource.h"

// TODO Should file be opened this long?
FileSource::FileSource(const std::string& fileName_) {
    file.open(fileName_);
    if (!file.is_open()) {
        throw FileException();
    }
    file.unsetf(std::ios_base::skipws); // We'll want to catch exact positions of characters

    char currentChar;
    // Try to read first two characters, and set them to eof if they're not present
    if (!(file >> currentChar)) {
        setCurrentChar(std::char_traits<char>::eof());
        setNextChar(std::char_traits<char>::eof());
    } else {
        setCurrentChar(currentChar);
        if (!(file >> currentChar)) {
            setNextChar(std::char_traits<char>::eof());
        } else {
            setNextChar(currentChar);
        }
    }
    // Set position of first character (which obviously should be 0:0)
    setLineNumber(0);
    setCharPos(0);
}

// TODO Detecting new line by using \n is probably not an answer as windows uses \r\n, think about it
void FileSource::goNext() {
    char newChar;
    setCurrentChar(peek()); // set peeked character as current character

    if (getChar() == '\n') {
        setLineNumber(getLineNumber() + 1);
        setCharPos(0);
    } else {
        setCharPos(getCharPos() + 1);
    }

    if (!(file >> newChar)) {
        setNextChar(std::char_traits<char>::eof());
    } else {
        setNextChar(newChar);
    }
}
