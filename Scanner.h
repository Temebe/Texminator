#ifndef TEXMINATOR_SCANNER_H
#define TEXMINATOR_SCANNER_H

#include <memory>

#include "Source.h"
#include "Token.h"

class Scanner {
public:
    Scanner(std::unique_ptr<Source> source_);
    Token nextToken();

private:
    bool isKeyword(const std::string &word_);

    std::unique_ptr<Source> source;
};


#endif //TEXMINATOR_SCANNER_H
