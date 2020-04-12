#ifndef TEXMINATOR_SCANNER_H
#define TEXMINATOR_SCANNER_H

#include <memory>

#include "Source.h"

class Scanner {
public:
    Scanner(std::unique_ptr<Source> source_);

private:
    std::unique_ptr<Source> source;
};


#endif //TEXMINATOR_SCANNER_H
