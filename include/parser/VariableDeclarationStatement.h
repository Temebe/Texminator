#ifndef TEXMINATOR_VARIABLEDECLARATIONSTATEMENT_H
#define TEXMINATOR_VARIABLEDECLARATIONSTATEMENT_H

#include "Statement.h"
#include "Expression.h"

class VariableDeclarationStatement : public Statement {
public:
    explicit VariableDeclarationStatement(const std::string& name_, ValueEnum type_ = VOID);
    void execute(Environment& environment) override;
    void setAssignmentExpression(std::unique_ptr<Expression> expression_);

private:
    std::string name;
    ValueEnum type;
    std::unique_ptr<Expression> assignmentExpression;
};


#endif //TEXMINATOR_VARIABLEDECLARATIONSTATEMENT_H
