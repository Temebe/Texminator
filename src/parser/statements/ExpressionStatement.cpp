#include "parser/statements/ExpressionStatement.h"

ExpressionStatement::ExpressionStatement(std::string identifier_, TokenType operationType_,
                                         std::unique_ptr<Expression> expression_)
                                         : identifier(std::move(identifier_)),
                                           operationType(operationType_),
                                           expression(std::move(expression_)) {}

void ExpressionStatement::execute(Environment &environment) {

}
