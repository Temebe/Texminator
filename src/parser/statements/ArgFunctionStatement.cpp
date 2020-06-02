#include "parser/statements/ArgFunctionStatement.h"

const std::string ArgFunctionStatement::argNumParam = "#argNum";

void ArgFunctionStatement::execute(Environment &environment) {
    auto val = castValue(environment.getVariable(argNumParam).value(), UNSIGNED_NUMBER);
    auto argNum = std::get<UnsignedNumberType>(val);
    if (argNum >= arguments.size()) {
        throw TexminatorException("Tried to access non existent argument");
    }

    environment.setReturnValue(arguments.at(argNum));
}
