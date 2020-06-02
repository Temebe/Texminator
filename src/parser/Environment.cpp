#include "parser/ParserException.h"
#include "parser/Environment.h"
#include "parser/statements/ArgFunctionStatement.h"

#include <algorithm>
#include <parser/statements/GetFunctionStatement.h>

const std::string Environment::returnValueName = "#return";



Environment::Environment(const std::vector<std::string> &args_) {
    addGlobalVariable("arg_count", castValue(args_.size(), UNSIGNED_NUMBER));
    initializeArgFunction(args_);
    initializeStandardStreams();
    initializeGetFunction();
}

Environment::Environment() : Environment(std::vector<std::string>()) {}

void Environment::createNewScope(const ScopeType type) {
    scopes.push_front(Scope(type));
}

void Environment::initializeArgFunction(const std::vector<std::string> &args_) {
    auto argFuncBody = std::make_unique<ArgFunctionStatement>(args_);
    Function argFunc(std::move(argFuncBody));
    argFunc.addParameter({ArgFunctionStatement::argNumParam, UNSIGNED_NUMBER});
    argFunc.setReturnType(STRING);
    addGlobalFunction("arg", argFunc);
}

void Environment::initializeStandardStreams() {
    addGlobalVariable("stdin", std::make_shared<StandardInput>());
    addGlobalVariable("stdout", std::make_shared<StandardOutput>());
}

void Environment::initializeGetFunction() {
    auto getFuncBody = std::make_unique<GetFunctionStatement>();
    Function getFunc(std::move(getFuncBody));
    getFunc.addParameter({GetFunctionStatement::toMatchParameterName, STRING});
    getFunc.addParameter({GetFunctionStatement::regexParameterName, STRING});
    getFunc.setReturnType(STRING);
    addGlobalFunction("get", getFunc);
}

// TODO Why do I return optional if I throw exception anyway? Refactor this
std::optional<Value> Environment::getVariable(const std::string &name_) {
    auto currentScope = scopes.begin();
    std::optional<Value> result;

    while (currentScope != scopes.end()) {
        result = currentScope->getVariable(name_);
        if ((currentScope++)->getType() == ScopeType::function || result) {
            break;
        }
    }

    if (!result) {
        result = globalScope.getVariable(name_);
    }

    if (result) {
        return result;
    }

    throw ParserException("Variable " + name_ + " does not exist");
}


void Environment::addVariable(const std::string &name_, const Value &variable_) {
    auto currentScope = scopes.begin();
    currentScope->addVariable(name_, variable_);
}

void Environment::addFunction(const std::string &name_, Function &function_) {
    auto currentScope = scopes.begin();
    currentScope->addFunction(name_, function_);
}

void Environment::addGlobalVariable(const std::string &name_, const Value &variable_) {
    globalScope.addVariable(name_, variable_);
}

void Environment::addGlobalFunction(const std::string &name_, Function &function_) {
    globalScope.addFunction(name_, function_);
}

void Environment::destroyCurrentScope() {
    scopes.pop_front();
}

Function& Environment::getFunction(const std::string &name_, const std::list<Parameter> &parameters_) {
    auto it = scopes.begin();
    for (; it != scopes.end(); ++it) {
        if (it->containsFunction(name_, parameters_)) {
            return it->getFunction(name_, parameters_);
        }
    }

    if (globalScope.containsFunction(name_, parameters_)) {
        return globalScope.getFunction(name_, parameters_);
    }

    throw ParserException("Function " + name_ + " with given parameters does not exist");
}

Function &Environment::getFunction(const std::string &name_, const std::list<Value>& parameters_) {
    std::list<Parameter> parameters;
    for (const auto& val : parameters_) {
        parameters.emplace_back("", static_cast<ValueEnum>(val.index()));
    }

    return getFunction(name_, parameters);
}

void Environment::setVariable(const std::string &name_, const Value &variable_) {
    auto currentScope = scopes.begin();

    while (currentScope != scopes.end()) {
        if (auto currentVariable = currentScope->getVariable(name_)) {
            auto varType = static_cast<ValueEnum>(currentVariable->index());
            currentScope->replaceVariable(name_, castValue(variable_, varType));
            return;
        }

        if ((currentScope++)->getType() == ScopeType::function) {
            break;
        }
    }

    throw ParserException("Variable " + name_ + " does not exist");
}

void Environment::setReturnValue(const Value &value_) {
    auto functionScope = std::find_if(scopes.begin(), scopes.end(),
            [](const Scope& scope) { return scope.getType() == function; });

    if (functionScope == scopes.end()) {
        throw TexminatorException("Tried to return not from function!");
    }

    functionScope->addVariable(returnValueName, value_);
}

Value Environment::getReturnValue() {
    auto functionScope = std::find_if(scopes.begin(), scopes.end(),
                                      [](const Scope& scope) { return scope.getType() == function; });

    if (functionScope == scopes.end()) {
        throw TexminatorException("Tried to return not from function!");
    }

    auto returnValue = functionScope->getVariable(returnValueName);
    if (!returnValue) {
        throw TexminatorException("Could not find a return value");
    }

    return *returnValue;
}
