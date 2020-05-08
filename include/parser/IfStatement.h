#ifndef TEXMINATOR_IFSTATEMENT_H
#define TEXMINATOR_IFSTATEMENT_H

#include "Statement.h"

class IfStatement : public Statement {
public:
    void execute(Environment &environment) override;

};


#endif //TEXMINATOR_IFSTATEMENT_H
