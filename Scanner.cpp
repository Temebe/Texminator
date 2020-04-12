#include "Scanner.h"

Scanner::Scanner(std::unique_ptr<Source> source) {
    this->source = std::move(source);
}
