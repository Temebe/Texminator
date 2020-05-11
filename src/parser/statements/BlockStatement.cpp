#include "parser/statements/BlockStatement.h"

void BlockStatement::execute(Environment &environment) {
    environment.createNewScope(ScopeType::local);
    for (const auto &statement : statements) {
        statement->execute(environment);

        if (statement->getExecStatus() != ExecStatus::ok) {
            this->execStatus = statement->getExecStatus();
            break;
        }
    }
    environment.destroyCurrentScope();
}
