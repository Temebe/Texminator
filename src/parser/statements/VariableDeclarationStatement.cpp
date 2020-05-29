#include "parser/statements/VariableDeclarationStatement.h"

#include "TexminatorExceptions.h"

VariableDeclarationStatement::VariableDeclarationStatement(const std::string& name_, const ValueEnum type_)
    : name(name_),
      type(type_) {}

void VariableDeclarationStatement::execute(Environment &environment) {
    Value value = getDefaultValue(type);
    if (assignmentExpression) {
        value = castValue(assignmentExpression->evaluate(environment), type);
        if (std::holds_alternative<VoidType>(value) && type != VOID) {
            throw WrongTypeException("Tried to assign wrong expression to variable");
        }
    }
    environment.addVariable(name, value);
}

void VariableDeclarationStatement::setAssignmentExpression(std::unique_ptr<Expression> expression_) {
    assignmentExpression = std::move(expression_);
}

const std::string &VariableDeclarationStatement::getName() const {
    return name;
}

ValueEnum VariableDeclarationStatement::getType() const {
    return type;
}

const std::unique_ptr<Expression> &VariableDeclarationStatement::getAssignmentExpression() const {
    return assignmentExpression;
}
