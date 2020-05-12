#include "parser/statements/ReturnStatement.h"

void ReturnStatement::execute(Environment &environment) {

}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> exp_)
    : exp(std::move(exp_)) {}
