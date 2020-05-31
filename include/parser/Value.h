#ifndef TEXMINATOR_VALUE_H
#define TEXMINATOR_VALUE_H

#include <variant>
#include <optional>
#include <TexminatorExceptions.h>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

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

// TODO namespace? Class?

static std::string valueEnumToString(const ValueEnum value_) {
    switch (value_) {
        case UNSIGNED_NUMBER:
            return "unsigned number";

        case NUMBER:
            return "number";

        case CHAR:
            return "char";

        case STRING:
            return "string";

        case FLOAT:
            return "float";

        case BOOL:
            return "bool";

        case VOID:
            return "void";
    }
}

static std::string valueToString(const Value &value_) {
    auto value = static_cast<ValueEnum>(value_.index());
    return valueEnumToString(value);
}

static Value getDefaultValue(const ValueEnum type_) {
    Value result;
    switch (type_) {
        case UNSIGNED_NUMBER:
            result.emplace<UnsignedNumberType>(0);
            break;
        case NUMBER:
            result.emplace<NumberType>(0);
            break;
        case CHAR:
            result.emplace<CharType>(0);
            break;
        case STRING:
            result.emplace<StringType>("");
            break;
        case FLOAT:
            result.emplace<FloatType>(0);
            break;
        case BOOL:
            result.emplace<BoolType>(false);
            break;
        case VOID:
            return Value(VoidType());
    }
    return result;
}

// TODO is this necessary / can it be done better?
static Value castValue(const Value &val_, const ValueEnum type_) {
    Value result = val_;
    std::visit(overload{
            [type_, &result](const UnsignedNumberType &v) {
                switch (type_) {
                    case UNSIGNED_NUMBER:
                        break;
                    case NUMBER:
                        result.emplace<NumberType>(v);
                        break;
                    case FLOAT:
                        result.emplace<FloatType>(v);
                        break;
                    case BOOL:
                        result.emplace<BoolType>(v);
                        break;
                    default:
                        result.emplace<VoidType>();
                }},

            [type_, &result](const NumberType &v) {
                switch (type_) {
                    case UNSIGNED_NUMBER:
                        result.emplace<UnsignedNumberType>(v);
                        break;
                    case NUMBER:
                        break;
                    case FLOAT:
                        result.emplace<FloatType>(v);
                        break;
                    case BOOL:
                        result.emplace<BoolType>(v);
                        break;
                    default:
                        result.emplace<VoidType>();
                }},

            [type_, &result](const CharType &v) {
                switch (type_) {
                    case CHAR:
                        break;
                    case STRING:
                        result.emplace<StringType>(&v);
                        break;
                    default:
                        result.emplace<VoidType>();
                }},

            [type_, &result](const StringType &v) {
                switch (type_) {
                    case CHAR:
                        if (v.size() == 1) {
                            result.emplace<CharType>(*v.begin());
                        } else {
                            result.emplace<VoidType>();
                        }
                        break;
                    case STRING:
                        break;
                    default:
                        result.emplace<VoidType>();
                }},

            [type_, &result](const FloatType &v) {
                switch (type_) {
                    case UNSIGNED_NUMBER:
                        result.emplace<UnsignedNumberType>();
                        break;
                    case NUMBER:
                        result.emplace<NumberType>(v);
                        break;
                    case FLOAT:
                        break;
                    case BOOL:
                        result.emplace<BoolType>(v);
                        break;
                    default:
                        result.emplace<VoidType>();
                }},

            [type_, &result](const BoolType &v) {
                switch (type_) {
                    case UNSIGNED_NUMBER:
                        result.emplace<UnsignedNumberType>(v);
                        break;
                    case NUMBER:
                        result.emplace<NumberType>(v);
                        break;
                    case FLOAT:
                        result.emplace<FloatType>(v);
                        break;
                    case BOOL:
                        break;
                    default:
                        result.emplace<VoidType>();
                }},

            [type_, &result](const auto &v) { result.emplace<VoidType>(); },
    }, val_);

    // It is assumed that if cast was not meant to be to void type but result is a void, then there was a bad cast
    if (!std::holds_alternative<VoidType>(result) && type_ != VOID) {
        throw BadCastException("Tried to cast from " + valueToString(val_) +
            " type to " + valueEnumToString(type_) + " type.");
    }

    return result;
}


#endif //TEXMINATOR_VALUE_H
