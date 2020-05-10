#ifndef TEXMINATOR_PARSEERROR_H
#define TEXMINATOR_PARSEERROR_H

#include <string>

struct ParseError {
    std::string err = ""; /** Reason for error */
    unsigned int line = 0; /** Line in which token is */
    unsigned int pos = 0;  /** Position in line */
};

#endif //TEXMINATOR_PARSEERROR_H
