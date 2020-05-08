#ifndef TEXMINATOR_STATEMENT_H
#define TEXMINATOR_STATEMENT_H

#include "Environment.h"

class Statement {
public:
    virtual void execute(Environment& environment) = 0;
};


#endif //TEXMINATOR_STATEMENT_H
