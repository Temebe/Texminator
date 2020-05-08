#ifndef TEXMINATOR_CONTINUESTATEMENT_H
#define TEXMINATOR_CONTINUESTATEMENT_H

#include "Statement.h"

class ContinueStatement : public Statement {
public:
    void execute(Environment &environment) override;

};


#endif //TEXMINATOR_CONTINUESTATEMENT_H
