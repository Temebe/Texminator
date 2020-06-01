#include "parser/statements/FunctionDeclarationStatement.h"

#include "parser/Function.h"

FunctionDeclarationStatement::FunctionDeclarationStatement(std::string name_, std::unique_ptr<Statement> body_,
                                                           std::list<Parameter> arguments_, ValueEnum returnType_)
                                                           : body(std::move(body_)),
                                                             arguments(std::move(arguments_)),
                                                             returnType(returnType_),
                                                             name(std::move(name_)) {}

void FunctionDeclarationStatement::execute(Environment &environment) {
    auto newFun = Function(std::move(body));
    newFun.setParameters(arguments);
    newFun.setReturnType(returnType);
    environment.addFunction(name, newFun);
}
