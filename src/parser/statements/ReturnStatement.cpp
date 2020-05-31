#include "parser/statements/ReturnStatement.h"

void ReturnStatement::execute(Environment &environment) {
    if (exp) {
        auto result = exp->evaluate(environment);
        environment.setReturnValue(result);
    } else {
        environment.setReturnValue(VoidType());
    }
    execStatus = returned;
}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> exp_)
    : exp(std::move(exp_)) {}
