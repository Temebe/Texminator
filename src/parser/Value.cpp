#include "parser/Value.h"

std::optional<UnsignedNumberType> Value::getUnsignedNumber() const {
    try {
        return std::get<UnsignedNumberType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<UnsignedNumberType>();
    }
}

std::optional<NumberType> Value::getNumber() const {
    try {
        return std::get<NumberType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<NumberType>();
    }
}

std::optional<CharType> Value::getChar() const {
    try {
        return std::get<CharType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<CharType>();
    }
}

std::optional<StringType> Value::getString() const {
    try {
        return std::get<StringType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<StringType>();
    }
}

std::optional<FloatType> Value::getFloat() const {
    try {
        return std::get<FloatType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<FloatType>();
    }
}

std::optional<BoolType> Value::getBool() const {
    try {
        return std::get<BoolType>(val);
    } catch (const std::bad_variant_access&) {
        return std::optional<BoolType>();
    }
}

// TODO This is ugly, change that
bool Value::isVoid() const {
    return val.index() == 6;
}

const ValueType& Value::getValue() const {
    return val;
}

void Value::setVal(const ValueType val_) {
    Value::val = val_;
}
