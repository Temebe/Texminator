#include "parser/statements/IfStatement.h"

#include "TexminatorExceptions.h"

IfStatement::IfStatement(std::unique_ptr<Expression> condition_, std::unique_ptr<Statement> trueStatement_,
                         std::unique_ptr<Statement> falseStatement_)
                         : condition(std::move(condition_)),
                           trueStatement(std::move(trueStatement_)),
                           falseStatement(std::move(falseStatement_)) {}

void IfStatement::execute(Environment &environment) {
    auto conditionValue = condition->evaluate(environment);
    if (!std::holds_alternative<BoolType>(conditionValue)) {
        throw EvaluationErrorException("Unable to evaluate if condition as bool type");
    }
    if (!trueStatement) {
        throw TexminatorException("If statement does not have a body");
    }

    if (std::get<BoolType>(conditionValue)) {
        trueStatement->execute(environment);
        execStatus = trueStatement->getExecStatus();
    } else {
        if (falseStatement) {
            falseStatement->execute(environment);
            execStatus = falseStatement->getExecStatus();
        }
    }
}
