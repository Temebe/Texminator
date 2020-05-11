#ifndef TEXMINATOR_IFSTATEMENT_H
#define TEXMINATOR_IFSTATEMENT_H

#include <parser/expressions/Expression.h>
#include "Statement.h"

class IfStatement : public Statement {
public:
    IfStatement(std::unique_ptr<Expression> condition_, std::unique_ptr<Statement> trueStatement_,
                std::unique_ptr<Statement> falseStatement_ = std::unique_ptr<Statement>());
    void execute(Environment &environment) override;

private:
    std::unique_ptr<Statement> trueStatement;
    std::unique_ptr<Statement> falseStatement;
    std::unique_ptr<Expression> condition;

};


#endif //TEXMINATOR_IFSTATEMENT_H
