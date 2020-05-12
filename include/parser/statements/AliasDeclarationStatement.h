#ifndef TEXMINATOR_ALIASDECLARATIONSTATEMENT_H
#define TEXMINATOR_ALIASDECLARATIONSTATEMENT_H


#include "Statement.h"

class AliasDeclarationStatement : public Statement {
public:
    AliasDeclarationStatement(std::string  alias_, std::string  underlying_);
    void execute(Environment& environment) override;

private:
    std::string alias;
    std::string underlyingVariable;
};


#endif //TEXMINATOR_ALIASDECLARATIONSTATEMENT_H
