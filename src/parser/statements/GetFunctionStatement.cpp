#include "parser/statements/GetFunctionStatement.h"

#include <regex>

const std::string GetFunctionStatement::toMatchParameterName = "#toMatch";
const std::string GetFunctionStatement::regexParameterName = "#regex";


void GetFunctionStatement::execute(Environment &environment) {
    auto toMatchVal = castValue(environment.getVariable(toMatchParameterName).value(), STRING);
    auto regexVal = castValue(environment.getVariable(regexParameterName).value(), STRING);
    const std::string toMatch = std::get<std::string>(toMatchVal);
    const std::string regexString = std::get<std::string>(regexVal);

    std::regex rgx(regexString);
    std::smatch match;
    if (std::regex_search(toMatch.begin(), toMatch.end(), match, rgx)) {
        environment.setReturnValue(match[1]);
        return;
    }

    environment.setReturnValue(getDefaultValue(STRING));
}
