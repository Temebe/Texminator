#ifndef TEXMINATOR_FORSTATEMENT_H
#define TEXMINATOR_FORSTATEMENT_H


#include "Statement.h"
#include "parser/expressions/Expression.h"

enum ForType {
    perCharacter, perLine
};

class ForStatement : public Statement {
public:
    explicit ForStatement(std::string itName_, std::unique_ptr<Expression> source_,
                          ForType type_, std::unique_ptr<Statement> body_);
    void execute(Environment& environment) override;

private:
    void executePerCharacter(Environment &environment, StreamType& stream_);
    void executePerLine(Environment &environment, StreamType& stream_);

    std::unique_ptr<Statement> body;
    const std::string iteratorName;
    std::unique_ptr<Expression> source;
    const ForType type;
};


#endif //TEXMINATOR_FORSTATEMENT_H
