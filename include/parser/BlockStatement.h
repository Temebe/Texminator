#ifndef TEXMINATOR_BLOCKSTATEMENT_H
#define TEXMINATOR_BLOCKSTATEMENT_H

#include <list>

#include "Statement.h"

class BlockStatement : public Statement {
public:
    void execute(Environment &environment) override;

private:
    std::list<std::unique_ptr<Statement>> statements;

};


#endif //TEXMINATOR_BLOCKSTATEMENT_H
