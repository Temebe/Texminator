#include <iostream>
#include <string>

#include "FileSource.h"

/* TODO Should file be opened this long? */
FileSource::FileSource(const std::string& fileName_) {
    file.open(fileName_);
    if (!file.is_open()) {
        throw FileException();
    }

    char currentChar;
    // Try to read first two characters, and set them to eof if they're not present
    if (!(file >> currentChar)) {
        setCurrentChar(std::char_traits<char>::eof());
        setNextChar(std::char_traits<char>::eof());
    } else if (!(file >> currentChar)) {
        setNextChar(std::char_traits<char>::eof());
    }

}

void FileSource::next() {

}
