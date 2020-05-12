#include "parser/statements/WriteStatement.h"

#include <utility>

WriteStatement::WriteStatement(std::string identifier_, std::unique_ptr<Expression> expression_)
    : identifier(std::move(identifier_)),
      expression(std::move(expression_)) {}

void WriteStatement::execute(Environment &environment) {

}

