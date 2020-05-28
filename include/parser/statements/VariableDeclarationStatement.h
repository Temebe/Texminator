#ifndef TEXMINATOR_VARIABLEDECLARATIONSTATEMENT_H
#define TEXMINATOR_VARIABLEDECLARATIONSTATEMENT_H

#include "Statement.h"
#include "parser/expressions/Expression.h"

class VariableDeclarationStatement : public Statement {
public:
    explicit VariableDeclarationStatement(const std::string& name_, ValueEnum type_ = VOID);
    void execute(Environment& environment) override;
    void setAssignmentExpression(std::unique_ptr<Expression> expression_);

    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] ValueEnum getType() const;
    [[nodiscard]] const std::unique_ptr<Expression> &getAssignmentExpression() const;

private:
    std::string name;
    ValueEnum type;
    std::unique_ptr<Expression> assignmentExpression;
};


#endif //TEXMINATOR_VARIABLEDECLARATIONSTATEMENT_H
