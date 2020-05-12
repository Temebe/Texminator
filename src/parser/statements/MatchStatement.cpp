#include "parser/statements/MatchStatement.h"

MatchStatement::MatchStatement(std::unique_ptr<Expression> matchExp_, MatchList matchList_,
        std::unique_ptr<Statement> any_)
        : matchExp(std::move(matchExp_)),
          matchList(std::move(matchList_)),
          anyStatement(std::move(any_)) {}

void MatchStatement::execute(Environment &environment) {

}
