#ifndef TEXMINATOR_BREAKSTATEMENT_H
#define TEXMINATOR_BREAKSTATEMENT_H

#include "Statement.h"

class BreakStatement : public Statement {
public:
    void execute(Environment &environment) override;

};


#endif //TEXMINATOR_BREAKSTATEMENT_H
