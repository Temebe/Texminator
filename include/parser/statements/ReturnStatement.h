#ifndef TEXMINATOR_RETURNSTATEMENT_H
#define TEXMINATOR_RETURNSTATEMENT_H

#include <parser/expressions/Expression.h>
#include "Statement.h"

class ReturnStatement : public Statement {
public:
    explicit ReturnStatement(std::unique_ptr<Expression> exp_);
    void execute(Environment &environment) override;

private:
    std::unique_ptr<Expression> exp;
};


#endif //TEXMINATOR_RETURNSTATEMENT_H
