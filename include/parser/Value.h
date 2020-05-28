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
struct VoidType {};
using Value = std::variant<UnsignedNumberType, NumberType,
                               CharType,           StringType,
                               FloatType,          BoolType,
                               VoidType>;

enum ValueEnum {
    UNSIGNED_NUMBER, NUMBER,
    CHAR,            STRING,
    FLOAT,           BOOL,
    VOID
};

static_assert(std::is_same_v<UnsignedNumberType, std::variant_alternative_t<UNSIGNED_NUMBER, Value>>);
static_assert(std::is_same_v<NumberType,         std::variant_alternative_t<NUMBER, Value>>);
static_assert(std::is_same_v<CharType,           std::variant_alternative_t<CHAR, Value>>);
static_assert(std::is_same_v<StringType,         std::variant_alternative_t<STRING, Value>>);
static_assert(std::is_same_v<FloatType,          std::variant_alternative_t<FLOAT, Value>>);
static_assert(std::is_same_v<BoolType,           std::variant_alternative_t<BOOL, Value>>);
static_assert(std::is_same_v<VoidType,           std::variant_alternative_t<VOID, Value>>);


#endif //TEXMINATOR_VALUE_H
