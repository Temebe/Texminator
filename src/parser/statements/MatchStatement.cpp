#include "parser/statements/MatchStatement.h"

#include "parser/expressions/ComparisonExpressions.h"
#include "parser/expressions/LiteralExpression.h"

MatchStatement::MatchStatement(std::unique_ptr<Expression> matchExp_, MatchList matchList_,
        std::unique_ptr<Statement> any_, std::unique_ptr<Statement> none_)
        : matchExp(std::move(matchExp_)),
          matchList(std::move(matchList_)),
          anyStatement(std::move(any_)),
          noneStatement(std::move(none_)) {}

void MatchStatement::execute(Environment &environment) {
    const Value matchValue = matchExp->evaluate(environment);
    bool matched = false;

    for (auto &pair : matchList) {
        auto expValue = pair.first->evaluate(environment);
        auto compareExp = std::make_unique<EqualExpression>();
        compareExp->setLeftExpression(std::make_unique<LiteralExpression>(matchValue));
        compareExp->setRightExpression(std::make_unique<LiteralExpression>(expValue));

        if (std::get<BoolType>(compareExp->evaluate(environment))) {
            matched = true;
            pair.second->execute(environment);
            break;
        }
    }

    if (matched) {
        if (anyStatement) {
            anyStatement->execute(environment);
        }
    } else {
        if (noneStatement) {
            noneStatement->execute(environment);
        }
    }
}
