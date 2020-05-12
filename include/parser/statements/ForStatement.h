#ifndef TEXMINATOR_FORSTATEMENT_H
#define TEXMINATOR_FORSTATEMENT_H


#include "Statement.h"

enum ForType {
    perCharacter, perLine
};

class ForStatement : public Statement {
public:
    explicit ForStatement(std::string itName_, std::string sourceName_,
                          ForType type_, std::unique_ptr<Statement> body_);
    void execute(Environment& environment) override;

private:
    std::unique_ptr<Statement> body;
    const std::string iteratorName;
    const std::string sourceName;
    const ForType type;
};


#endif //TEXMINATOR_FORSTATEMENT_H
