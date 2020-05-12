#ifndef TEXMINATOR_FUNCTIONDECLARATIONSTATEMENT_H
#define TEXMINATOR_FUNCTIONDECLARATIONSTATEMENT_H

#include "Statement.h"

class FunctionDeclarationStatement : public Statement {
public:
    explicit FunctionDeclarationStatement(std::string name, std::unique_ptr<Statement> body_,
                                          std::list<Parameter> arguments_,ValueEnum returnType_ = VOID);
    void execute(Environment& environment) override;

private:
    std::string name;
    std::list<Parameter> arguments;
    ValueEnum returnType;
    std::unique_ptr<Statement> body;
};


#endif //TEXMINATOR_FUNCTIONDECLARATIONSTATEMENT_H
