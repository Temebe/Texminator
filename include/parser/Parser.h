#ifndef TEXMINATOR_PARSER_H
#define TEXMINATOR_PARSER_H

#include <memory>

#include "Scanner.h"
#include "Environment.h"

class Parser {
public:
    void parse(Scanner& scanner_);

private:
    Environment environment;
};


#endif //TEXMINATOR_PARSER_H
