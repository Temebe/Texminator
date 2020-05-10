#ifndef TEXMINATOR_BLOCKSTATEMENT_H
#define TEXMINATOR_BLOCKSTATEMENT_H

#include <list>

#include "Statement.h"
#include "Environment.h"
#include "FunctionBody.h"

class BlockStatement : public Statement, public FunctionBody {
public:
    void execute(Environment &environment) override;

private:
    std::list<std::unique_ptr<Statement>> statements;

};


#endif //TEXMINATOR_BLOCKSTATEMENT_H
