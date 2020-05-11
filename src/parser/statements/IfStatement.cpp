#include "parser/statements/IfStatement.h"

IfStatement::IfStatement(std::unique_ptr<Expression> condition_, std::unique_ptr<Statement> trueStatement_,
                         std::unique_ptr<Statement> falseStatement_) {

}

void IfStatement::execute(Environment &environment) {

}
