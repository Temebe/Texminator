#ifndef TEXMINATOR_WRITESTATEMENT_H
#define TEXMINATOR_WRITESTATEMENT_H


#include <parser/expressions/Expression.h>
#include "Statement.h"

class WriteStatement : public Statement {
public:
    explicit WriteStatement(std::string identifier_);
    WriteStatement(std::string identifier_, std::unique_ptr<Expression> expression_);
    void execute(Environment& environment) override;

private:
    const std::string identifier;
    const std::unique_ptr<Expression> expression;
    const bool newline = false;
};


#endif //TEXMINATOR_WRITESTATEMENT_H
