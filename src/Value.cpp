#include "../include/Value.h"

std::optional<UnsignedNumberType> Value::getUnsignedNumber() {
    try {
        return std::get<UnsignedNumberType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<UnsignedNumberType>();
    }
}

std::optional<NumberType> Value::getNumber() {
    try {
        return std::get<NumberType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<NumberType>();
    }
}

std::optional<CharType> Value::getChar() {
    try {
        return std::get<CharType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<CharType>();
    }
}

std::optional<StringType> Value::getString() {
    try {
        return std::get<StringType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<StringType>();
    }
}

std::optional<FloatType> Value::getFloat() {
    try {
        return std::get<FloatType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<FloatType>();
    }
}

std::optional<BoolType> Value::getBool() {
    try {
        return std::get<BoolType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<BoolType>();
    }
}
