#include "parser/statements/WriteStatement.h"

#include <utility>

WriteStatement::WriteStatement(std::string identifier_)
    : identifier(std::move(identifier_)),
      newline(true) {}

WriteStatement::WriteStatement(std::string identifier_, std::unique_ptr<Expression> expression_)
    : identifier(std::move(identifier_)),
      expression(std::move(expression_)) {}

void WriteStatement::execute(Environment &environment) {
    auto variable = environment.getVariable(identifier);
    if (!std::holds_alternative<StreamType>(variable.value())) {
        throw WrongTypeException("Tried to write to variable that is not a stream type");
    }

    auto stream = std::get<StreamType>(variable.value());

    if (newline) {
        stream->write("\n");
        return;
    }

    std::string toWrite = std::get<StringType>(castValue(expression->evaluate(environment), STRING));
    stream->write(toWrite);
}



