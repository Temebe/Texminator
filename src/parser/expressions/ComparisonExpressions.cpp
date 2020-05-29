#include <functional>
#include "parser/expressions/ComparisonExpressions.h"

template<typename T1, typename T2>
bool relationalCompare(const T1& l_, const T2& r_, const RelationType type_) {
    switch (type_) {
        case greaterEqual:
            return l_ >= r_;

        case greater:
            return l_ > r_;

        case lesser:
            return l_ < r_;

        case lesserEqual:
            return l_ <= r_;
    }
    return false;
}

Value EqualExpression::evaluate(Environment &environment) {
    if (!(leftExpression && rightExpression)) {
        return Value(VoidType());
    }
    Value leftValue = leftExpression->evaluate(environment);
    Value rightValue = leftExpression->evaluate(environment);

    Value result = std::visit(overload {
            [](UnsignedNumberType &l, UnsignedNumberType &r){ return Value(l == r); },
            [](UnsignedNumberType &l, NumberType &r){ return Value(l == r); },
            [](UnsignedNumberType &l, FloatType &r){ return Value(l == r); },
            [](UnsignedNumberType &l, BoolType &r){ return Value(l == r); },

            [](NumberType &l, UnsignedNumberType &r){ return Value(l == r); },
            [](NumberType &l, NumberType &r){ return Value(l == r); },
            [](NumberType &l, FloatType &r){ return Value(l == r); },
            [](NumberType &l, BoolType &r){ return Value(l == r); },

            [](CharType &l, CharType &r){ return Value(l == r); },
            [](CharType &l, StringType &r){ return Value(std::string(&l) == r); },

            [](StringType &l, CharType &r){ return Value(l == std::string(&r)); },
            [](StringType &l, StringType &r){ return Value(l == r); },

            [](FloatType &l, UnsignedNumberType &r){ return Value(l == r); },
            [](FloatType &l, NumberType &r){ return Value(l == r); },
            [](FloatType &l, FloatType &r){ return Value(l == r); },
            [](FloatType &l, BoolType &r){ return Value(l == r); },

            [](BoolType &l, UnsignedNumberType &r){ return Value(l == r); },
            [](BoolType &l, NumberType &r){ return Value(l == r); },
            [](BoolType &l, FloatType &r){ return Value(l == r); },

            [](auto &l, auto &r){ return Value(VoidType()); }
    }, leftValue, rightValue);

    if (std::holds_alternative<BoolType>(result) && negation) {
        result = !std::get<BoolType>(result);
    }

    return result;
}

Value RelationalExpression::evaluate(Environment &environment) {
    if (!(leftExpression && rightExpression)) {
        return Value(VoidType());
    }
    Value leftValue = leftExpression->evaluate(environment);
    Value rightValue = leftExpression->evaluate(environment);

    Value result = std::visit(overload {
            [this](UnsignedNumberType &l, UnsignedNumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](UnsignedNumberType &l, NumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](UnsignedNumberType &l, FloatType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](UnsignedNumberType &l, BoolType &r){ return Value(relationalCompare(l, r, relationType)); },

            [this](NumberType &l, UnsignedNumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](NumberType &l, NumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](NumberType &l, FloatType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](NumberType &l, BoolType &r){ return Value(relationalCompare(l, r, relationType)); },

            [this](FloatType &l, UnsignedNumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](FloatType &l, NumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](FloatType &l, FloatType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](FloatType &l, BoolType &r){ return Value(relationalCompare(l, r, relationType)); },

            [this](BoolType &l, UnsignedNumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](BoolType &l, NumberType &r){ return Value(relationalCompare(l, r, relationType)); },
            [this](BoolType &l, FloatType &r){ return Value(relationalCompare(l, r, relationType)); },

            [this](auto &l, auto &r){ return Value(VoidType()); }
    }, leftValue, rightValue);

    return result;
}


