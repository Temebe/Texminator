#ifndef TEXMINATOR_MATCHSTATEMENT_H
#define TEXMINATOR_MATCHSTATEMENT_H

#include <parser/expressions/Expression.h>
#include "Statement.h"

using MatchList = std::list<std::pair<std::unique_ptr<Expression>, std::unique_ptr<Statement>>>;

class MatchStatement : public Statement {
public:
    MatchStatement(std::unique_ptr<Expression> matchExp_, MatchList matchList_,
                   std::unique_ptr<Statement> any_ = std::unique_ptr<Statement>(),
                   std::unique_ptr<Statement> none_ = std::unique_ptr<Statement>());
    void execute(Environment& environment) override;

private:
    MatchList matchList;
    std::unique_ptr<Expression> matchExp;
    std::unique_ptr<Statement> anyStatement;
    std::unique_ptr<Statement> noneStatement;
};


#endif //TEXMINATOR_MATCHSTATEMENT_H
