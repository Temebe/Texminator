#include "parser/expressions/FormattedStringExpression.h"

FormattedStringExpression::FormattedStringExpression(std::unique_ptr<Expression> string_,
                                                     std::list<std::unique_ptr<Expression>> arguments_)
                                                     : stringExpression(std::move(string_)),
                                                       arguments(std::move(arguments_)) {}

Value FormattedStringExpression::evaluate(Environment &environment) {
    auto stringVal = stringExpression->evaluate(environment);
    if (!std::holds_alternative<StringType>(stringVal)) {
        throw WrongTypeException("Tried to format expression that is not string!");
    }

    auto toFormat = std::get<StringType>(castValue(stringVal, STRING));
    auto argIt = arguments.begin();
    size_t index = 0;
    while (index < toFormat.size()) {
        index = toFormat.find("{}", index);
        if (index == std::string::npos) {
            break;
        }

        auto valueToInsert = std::get<StringType>(formattedCast((*argIt++)->evaluate(environment)));
        if (valueToInsert.size() == 1) {
            valueToInsert += " "; // TODO Fix this later
        }
        toFormat.replace(index, valueToInsert.size(), valueToInsert);
        index += valueToInsert.size();
    }
    if (argIt != arguments.end()) {
        throw TexminatorException("Formatted string was given more arguments than {} was placed");
    }

    return toFormat;
}


