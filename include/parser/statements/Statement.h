#ifndef TEXMINATOR_STATEMENT_H
#define TEXMINATOR_STATEMENT_H

#include "parser/Environment.h"

enum ExecStatus {
    ok, jumped, broke, returned
};

class Statement {
public:
    ~Statement() = default;
    virtual void execute(Environment& environment) = 0;
    ExecStatus getExecStatus();

protected:
    ExecStatus execStatus = ok;

};


#endif //TEXMINATOR_STATEMENT_H
