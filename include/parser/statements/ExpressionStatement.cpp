#include "ExpressionStatement.h"

void ExpressionStatement::execute(Environment &environment) {
    expression->evaluate(environment);
}
