#include "../../include/parser/BlockStatement.h"

void BlockStatement::execute(Environment &environment) {
    environment.createNewScope(ScopeType::local);
    for (const auto &statement : statements) {
        statement->execute(environment);
    }
    environment.destroyCurrentScope();
}
