#ifndef TEXMINATOR_IFMATCHESSTATEMENT_H
#define TEXMINATOR_IFMATCHESSTATEMENT_H


#include <parser/expressions/Expression.h>
#include "Statement.h"

class IfMatchesStatement : public Statement {
public:
    explicit IfMatchesStatement(std::unique_ptr<Statement> trueStatement_, std::unique_ptr<Statement> falseStatement_,
                                std::string variableName_, std::unique_ptr<Expression> stringToMatch_);
    void execute(Environment &environment) override;

private:
    std::unique_ptr<Statement> trueStatement;
    std::unique_ptr<Statement> falseStatement;
    std::string variable;
    std::unique_ptr<Expression> stringToMatch;
};


#endif //TEXMINATOR_IFMATCHESSTATEMENT_H
