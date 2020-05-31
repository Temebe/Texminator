#include "parser/statements/BreakStatement.h"

void BreakStatement::execute(Environment &environment) {
    execStatus = broke;
}
