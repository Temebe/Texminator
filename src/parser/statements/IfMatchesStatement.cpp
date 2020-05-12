#include "parser/statements/IfMatchesStatement.h"

IfMatchesStatement::IfMatchesStatement(std::unique_ptr<Statement> trueStatement_,
                                       std::unique_ptr<Statement> falseStatement_, std::string variableName_,
                                       std::unique_ptr<Expression> stringToMatch_)
                                       : trueStatement(std::move(trueStatement_)),
                                         falseStatement(std::move(falseStatement_)),
                                         variable(std::move(variableName_)),
                                         stringToMatch(std::move(stringToMatch_)) {}

void IfMatchesStatement::execute(Environment &environment) {}
