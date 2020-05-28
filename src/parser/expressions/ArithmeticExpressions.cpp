#include "parser/expressions/ArithmeticExpressions.h"

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

struct VoidOperationException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Tried to use void as an value";
    }
};

struct WrongTypeException : public std::exception {
    WrongTypeException(const std::string &msg_) : msg(msg_) {}

    [[nodiscard]] const char* what() const noexcept override {
        return "Tried to use void as an value";
    }

private:
    std::string msg;
};

/** ADDITION */

Value AdditionExpression::evaluate(Environment &environment) {
    if (!(leftExpression && rightExpression)) {
        // TODO Handle missing expressions better
        return Value(VoidType());
    }
    Value leftValue = leftExpression->evaluate(environment);
    Value rightValue = leftExpression->evaluate(environment);

    // TODO Handle wrong type additions
    // For some reason I cannot throw exceptions and I am not sure yet why is that
    // For now I'll just return void type

    Value result = std::visit(overload {
        [this](UnsignedNumberType &l, UnsignedNumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](UnsignedNumberType &l, NumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](UnsignedNumberType &l, FloatType &r){ return Value(subtraction ? l - r : l + r); },
        [this](UnsignedNumberType &l, BoolType &r){ return Value(subtraction ? l - r : l + r); },

        [this](NumberType &l, UnsignedNumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](NumberType &l, NumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](NumberType &l, FloatType &r){ return Value(subtraction ? l - r : l + r); },
        [this](NumberType &l, BoolType &r){ return Value(subtraction ? l - r : l + r); },

        [this](CharType &l, CharType &r){ return subtraction ? Value(VoidType()) : Value(std::string() + l + r); },
        [this](CharType &l, StringType &r){ return subtraction ? Value(VoidType()) : Value(l + r); },

        [this](StringType &l, CharType &r){ return subtraction ? Value(VoidType()) : Value(l + r); },
        [this](StringType &l, StringType &r){ return subtraction ? Value(VoidType()) : Value(l + r); },

        [this](FloatType &l, UnsignedNumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](FloatType &l, NumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](FloatType &l, FloatType &r){ return Value(subtraction ? l - r : l + r); },
        [this](FloatType &l, BoolType &r){ return Value(subtraction ? l - r : l + r); },

        [this](BoolType &l, UnsignedNumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](BoolType &l, NumberType &r){ return Value(subtraction ? l - r : l + r); },
        [this](BoolType &l, FloatType &r){ return Value(subtraction ? l - r : l + r); },

        [this](auto &l, auto &r){ return Value(VoidType()); }
    }, leftValue, rightValue);

    return result;
}

AdditionExpression::AdditionExpression(const bool subtraction_)
    : subtraction(subtraction_) {}


/** MULTIPLICATION */

Value MultiplicationExpression::evaluate(Environment &environment) {
    if (!(leftExpression && rightExpression)) {
        return Value(VoidType());
    }
    Value leftValue = leftExpression->evaluate(environment);
    Value rightValue = leftExpression->evaluate(environment);

    Value result = std::visit(overload {
            [this](UnsignedNumberType &l, UnsignedNumberType &r){ return Value(division ? l / r : l * r); },
            [this](UnsignedNumberType &l, NumberType &r){ return Value(division ? l / r : l * r); },
            [this](UnsignedNumberType &l, FloatType &r){ return Value(division ? l / r : l * r); },
            [this](UnsignedNumberType &l, BoolType &r){ return Value(division ? l / r : l * r); },

            [this](NumberType &l, UnsignedNumberType &r){ return Value(division ? l / r : l * r); },
            [this](NumberType &l, NumberType &r){ return Value(division ? l / r : l * r); },
            [this](NumberType &l, FloatType &r){ return Value(division ? l / r : l * r); },
            [this](NumberType &l, BoolType &r){ return Value(division ? l / r : l * r); },

            [this](FloatType &l, UnsignedNumberType &r){ return Value(division ? l / r : l * r); },
            [this](FloatType &l, NumberType &r){ return Value(division ? l / r : l * r); },
            [this](FloatType &l, FloatType &r){ return Value(division ? l / r : l * r); },
            [this](FloatType &l, BoolType &r){ return Value(division ? l / r : l * r); },

            [this](BoolType &l, UnsignedNumberType &r){ return Value(division ? l / r : l * r); },
            [this](BoolType &l, NumberType &r){ return Value(division ? l / r : l * r); },
            [this](BoolType &l, FloatType &r){ return Value(division ? l / r : l * r); },

            [this](auto &l, auto &r){ return Value(VoidType()); }
    }, leftValue, rightValue);

    return result;
}

MultiplicationExpression::MultiplicationExpression(const bool division_)
    : division(division_) {}


/** MODULO */

Value ModuloExpression::evaluate(Environment &environment) {
    if (!(leftExpression && rightExpression)) {
        return Value(VoidType());
    }
    Value leftValue = leftExpression->evaluate(environment);
    Value rightValue = leftExpression->evaluate(environment);

    Value result = std::visit(overload {
            [this](UnsignedNumberType &l, UnsignedNumberType &r){ return Value(l % r); },
            [this](UnsignedNumberType &l, NumberType &r){ return Value(l % r); },

            [this](NumberType &l, UnsignedNumberType &r){ return Value(l % r); },
            [this](NumberType &l, NumberType &r){ return Value(l % r); },

            [this](auto &l, auto &r){ return Value(VoidType()); }
    }, leftValue, rightValue);

    return result;
}