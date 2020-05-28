#include "parser/statements/VariableDeclarationStatement.h"

VariableDeclarationStatement::VariableDeclarationStatement(const std::string& name_, const ValueEnum type_)
    : name(name_),
      type(type_) {}

void VariableDeclarationStatement::execute(Environment &environment) {

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
