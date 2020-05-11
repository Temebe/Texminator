#ifndef TEXMINATOR_RETURNSTATEMENT_H
#define TEXMINATOR_RETURNSTATEMENT_H

#include "Statement.h"

class ReturnStatement : public Statement{
public:
    void execute(Environment &environment) override;

};


#endif //TEXMINATOR_RETURNSTATEMENT_H
