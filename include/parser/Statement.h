#ifndef TEXMINATOR_STATEMENT_H
#define TEXMINATOR_STATEMENT_H

#include "Environment.h"

enum ExecStatus {
    ok, jumped, broke, returned
};

class Statement {
public:
    virtual void execute(Environment& environment) = 0;
    ExecStatus getExecStatus();

private:
    ExecStatus execStatus = ok;

};


#endif //TEXMINATOR_STATEMENT_H
