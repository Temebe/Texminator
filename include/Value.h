#ifndef TEXMINATOR_VALUE_H
#define TEXMINATOR_VALUE_H

#include <variant>
#include <optional>

using UnsignedNumberType = uint64_t;
using NumberType = int64_t;
using CharType = char;
using StringType = std::string;
using FloatType = double;
using BoolType = bool;
using ValueType = std::variant<UnsignedNumberType, NumberType,
                               CharType,           StringType,
                               FloatType,          BoolType>;

class Value {
public:
    std::optional<UnsignedNumberType> getUnsignedNumber();
    std::optional<NumberType> getNumber();
    std::optional<CharType> getChar();
    std::optional<StringType> getString();
    std::optional<FloatType> getFloat();
    std::optional<BoolType> getBool();

private:
    ValueType val;
};


#endif //TEXMINATOR_VALUE_H
