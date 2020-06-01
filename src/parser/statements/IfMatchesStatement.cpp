#include "parser/statements/IfMatchesStatement.h"

#include <regex>

IfMatchesStatement::IfMatchesStatement(std::unique_ptr<Statement> trueStatement_,
                                       std::unique_ptr<Statement> falseStatement_,
                                       std::unique_ptr<Expression> variableExp_,
                                       std::unique_ptr<Expression> regexExp_)
                                       : trueStatement(std::move(trueStatement_)),
                                         falseStatement(std::move(falseStatement_)),
                                         variableExp(std::move(variableExp_)),
                                         regexExp(std::move(regexExp_)) {}

void IfMatchesStatement::execute(Environment &environment) {
    auto regexString = castValue(regexExp->evaluate(environment), STRING);
    if (!std::holds_alternative<StringType>(regexString)) {
        throw EvaluationErrorException("Unable to evaluate if match's regex");
    }

    auto matchVariable = castValue(variableExp->evaluate(environment), STRING);

    if (!trueStatement) {
        throw TexminatorException("If statement does not have a body");
    }

    if (std::regex_match(std::get<StringType>(matchVariable), std::regex(std::get<StringType>(regexString)))) {
        trueStatement->execute(environment);
        execStatus = trueStatement->getExecStatus();

    } else {
        if (falseStatement) {
            falseStatement->execute(environment);
            execStatus = falseStatement->getExecStatus();
        }
    }

}
