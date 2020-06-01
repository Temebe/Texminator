#include "parser/expressions/FunctionCallExpression.h"
#include "parser/statements/Statement.h"

#include <utility>

FunctionCallExpression::FunctionCallExpression(std::string name_, std::list<std::unique_ptr<Expression>> arguments_)
    : name(std::move(name_)),
      arguments(std::move(arguments_)) {}

Value FunctionCallExpression::evaluate(Environment &environment) {
    std::list<Value> evaluatedArgs;
    for (auto &arg : arguments) {
        evaluatedArgs.emplace_back(arg->evaluate(environment));
    }

    auto &fun = environment.getFunction(name, evaluatedArgs);
    auto evalArgsIt = evaluatedArgs.begin();

    // First add to scope every parameter as a variable
    environment.createNewScope(function);
    for (const auto &param : fun.getParameters()) {
        if (evalArgsIt == evaluatedArgs.end()) {
            throw TexminatorException("Not enough arguments were provided for " + name + " function.");
        }
        environment.addVariable(param.first, castValue(*(evalArgsIt++), param.second));
    }

    fun.getBody()->execute(environment);
    auto result = environment.getReturnValue();
    if (fun.getReturnType() == VOID) {
        result = VoidType();
    } else {
        result = castValue(result, fun.getReturnType());
    }
    environment.destroyCurrentScope();

    return result;
}