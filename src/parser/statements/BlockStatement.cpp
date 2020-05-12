#include "parser/statements/BlockStatement.h"

BlockStatement::BlockStatement(std::list<std::unique_ptr<Statement>> statements_)
    : statements(std::move(statements_)) {}

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


