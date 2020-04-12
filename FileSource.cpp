#include "FileSource.h"

char FileSource::getChar() {
    return 0;
}

void FileSource::next() {

}

/* TODO Should file be opened this long? */
FileSource::FileSource(const std::string& fileName_) {
    file.open(fileName_);
    if (!file.is_open()) {
        throw FileException();
    }
}
