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

    auto argIt = arguments.begin();

    auto toFormat = std::get<StringType>(castValue(stringVal, STRING));
    for (auto it = toFormat.begin(); it != toFormat.end(); ++it) {
        if (*it != '{') {
            continue;
        }
        auto startErase = it;

        if (++it != toFormat.end()) {
            if (*it != '}') {
                continue;
            }
        }

        // We found a {} symbol
        if (argIt == arguments.end()) {
            throw TexminatorException("Formatted string ran out of arguments");
        }
        auto valueToInsert = std::get<StringType>(castValue((*argIt++)->evaluate(environment), STRING));
        it = toFormat.erase(startErase, ++it ); // erase {}
        toFormat.insert(it, valueToInsert.begin(), valueToInsert.end());
        it = it + (valueToInsert.size() - 1);
    }
    return toFormat;
}


