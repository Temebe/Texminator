#include "parser/Stream.h"
#include "TexminatorExceptions.h"

Stream::~Stream() {

}

OpenMode Stream::getMode() const {
    return mode;
}

/// ####################################
/// ########## STANDARD INPUT ##########
/// ####################################

void StandardInput::close() {

}

// This method should not be used on cin therefore just for sake of consistency
// read whole line and just return first letter
char StandardInput::readChar() {
    std::string line;
    std::cin >> line;
    return *line.begin();
}

char StandardInput::readChar(bool &ok_) {
    ok_ = true;
    return readChar();
}

std::string StandardInput::readLine() {
    std::string line;
    std::cin >> line;
    return line;
}

std::string StandardInput::readLine(bool &ok_) {
    ok_ = true;
    return readLine();
}

void StandardInput::nextChar() {

}

void StandardInput::nextLine() {

}

void StandardInput::write(const std::string &toWrite_) {
    throw BadStreamException("Tried to write to standard input");
}





/// #####################################
/// ########## STANDARD OUTPUT ##########
/// #####################################

void StandardOutput::close() {

}

char StandardOutput::readChar() {
    throw BadStreamException("Tried to read from standard output");
}

char StandardOutput::readChar(bool &ok_) {
    throw BadStreamException("Tried to read from standard output");
}

std::string StandardOutput::readLine() {
    throw BadStreamException("Tried to read from standard output");
}

std::string StandardOutput::readLine(bool &ok_) {
    ok_ = false;
    throw BadStreamException("Tried to read from standard output");
}

void StandardOutput::nextChar() {

}

void StandardOutput::nextLine() {

}

void StandardOutput::write(const std::string &toWrite_) {
    std::cout << toWrite_;
}




/// #################################
/// ########## FILE STREAM ##########
/// #################################

FileStream::FileStream(const std::string &fileName_, OpenMode mode_)
    : Stream(mode_),
      fileName(fileName_) {
    switch (mode_) {
        case readMode:
            file.open(fileName_, std::ios::in);
            break;

        case writeMode:
            file.open(fileName_, std::ios::out);
            break;

        case readwrite:
            file.open(fileName_, std::ios::in | std::ios::out);
            break;
    }

    if (!file.is_open()) {
        throw BadStreamException("Could not open " + fileName_);
    }
}

void FileStream::close() {
    file.close();
}

char FileStream::readChar() {
    assertGood();
    assertMode(readMode);

    char result;
    file.get(result);
    return result;
}

char FileStream::readChar(bool &ok_) {
    auto result = readChar();
    ok_ = !!file;
    return result;
}

std::string FileStream::readLine() {
    assertGood();
    assertMode(readMode);

    std::string result;
    std::getline(file, result);
    return result;
}

std::string FileStream::readLine(bool &ok_) {
    auto result = readLine();
    ok_ = !!file;
    return result;
}

// TODO Read about other methods, what if file is opened to readwrite?
void FileStream::nextChar() {
    assertGood();
    assertMode(readMode);

    file.get();
}

void FileStream::nextLine() {
    assertGood();
    assertMode(readMode);

    std::string temp;
    file >> temp;
}

void FileStream::write(const std::string &toWrite_) {
    assertGood();
    assertMode(writeMode);

    file << toWrite_;
}

void FileStream::assertGood() {
    if (!file) {
        throw BadStreamException("Unable to use " + fileName);
    }
}

void FileStream::assertMode(const OpenMode requiredMode_) {
    switch (getMode()) {

        case readMode:
            if (requiredMode_ == writeMode) {
                throw BadStreamException("Tried to write to read only file " + fileName);
            }
            break;

        case writeMode:
            if (requiredMode_ == readMode) {
                throw BadStreamException("Tried to read from write only file " + fileName);
            }
            break;

        case readwrite:
            break;
    }
}


/// ##################################
/// ########## EMPTY STREAM ##########
/// ##################################

EmptyStream::EmptyStream() : Stream(readwrite) {}

void EmptyStream::close() {}

char EmptyStream::readChar() {
    return 0;
}

std::string EmptyStream::readLine() {
    return std::string();
}

void EmptyStream::nextChar() {}

void EmptyStream::nextLine() {}


void EmptyStream::write(const std::string &toWrite_) {}

std::string EmptyStream::readLine(bool &ok_) {
    ok_ = true;
    return std::string();
}

char EmptyStream::readChar(bool &ok_) {
    ok_ = true;
    return 0;
}
