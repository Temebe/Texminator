#ifndef TEXMINATOR_VALUE_H
#define TEXMINATOR_VALUE_H

#include <variant>
#include <optional>

// TODO Is variant here a good choice? Should I get rid of enum?

using UnsignedNumberType = uint64_t;
using NumberType = int64_t;
using CharType = char;
using StringType = std::string;
using FloatType = double;
using BoolType = bool;
struct VoidType {};
using ValueType = std::variant<UnsignedNumberType, NumberType,
                               CharType,           StringType,
                               FloatType,          BoolType,
                               VoidType>;

enum ValueEnum {
    UNSIGNED_NUMBER, NUMBER,
    CHAR,            STRING,
    FLOAT,           BOOL,
    VOID
};

static_assert(std::is_same_v<UnsignedNumberType, std::variant_alternative_t<UNSIGNED_NUMBER, ValueType>>);
static_assert(std::is_same_v<NumberType,         std::variant_alternative_t<NUMBER, ValueType>>);
static_assert(std::is_same_v<CharType,           std::variant_alternative_t<CHAR, ValueType>>);
static_assert(std::is_same_v<StringType,         std::variant_alternative_t<STRING, ValueType>>);
static_assert(std::is_same_v<FloatType,          std::variant_alternative_t<FLOAT, ValueType>>);
static_assert(std::is_same_v<BoolType,           std::variant_alternative_t<BOOL, ValueType>>);
static_assert(std::is_same_v<VoidType,           std::variant_alternative_t<VOID, ValueType>>);

class Value {
public:
    const ValueType& getValue() const;
    std::optional<UnsignedNumberType> getUnsignedNumber() const;
    std::optional<NumberType> getNumber() const;
    std::optional<CharType> getChar() const;
    std::optional<StringType> getString() const;
    std::optional<FloatType> getFloat() const;
    std::optional<BoolType> getBool() const;
    bool isVoid() const;

    void setVal(ValueType val_);

private:
    ValueType val = VoidType();
};


#endif //TEXMINATOR_VALUE_H
