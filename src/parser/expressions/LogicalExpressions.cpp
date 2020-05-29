#include "parser/expressions/LogicalExpressions.h"

Value AndExpression::evaluate(Environment &environment) {
    if (!(leftExpression && rightExpression)) {
        return Value(VoidType());
    }
    Value leftValue = leftExpression->evaluate(environment);
    Value rightValue = rightExpression->evaluate(environment);

    Value result = std::visit(overload {
            [this](UnsignedNumberType &l, UnsignedNumberType &r){ return Value(static_cast<BoolType>(l && r)); },
            [this](UnsignedNumberType &l, NumberType &r){ return Value(static_cast<BoolType>(l && r)); },
            [this](UnsignedNumberType &l, BoolType &r){ return Value(static_cast<BoolType>(l && r)); },

            [this](NumberType &l, UnsignedNumberType &r){ return Value(static_cast<BoolType>(l && r)); },
            [this](NumberType &l, NumberType &r){ return Value(static_cast<BoolType>(l && r)); },
            [this](NumberType &l, BoolType &r){ return Value(static_cast<BoolType>(l && r)); },

            [this](BoolType &l, UnsignedNumberType &r){ return Value(static_cast<BoolType>(l && r)); },
            [this](BoolType &l, NumberType &r){ return Value(static_cast<BoolType>(l && r)); },
            [this](BoolType &l, BoolType &r){ return Value(l && r); },

            [this](auto &l, auto &r){ return Value(VoidType()); }
    }, leftValue, rightValue);

    return result;
}

Value OrExpression::evaluate(Environment &environment) {
    if (!(leftExpression && rightExpression)) {
        return Value(VoidType());
    }
    Value leftValue = leftExpression->evaluate(environment);
    Value rightValue = rightExpression->evaluate(environment);

    Value result = std::visit(overload {
            [this](UnsignedNumberType &l, UnsignedNumberType &r){ return Value(static_cast<BoolType>(l || r)); },
            [this](UnsignedNumberType &l, NumberType &r){ return Value(static_cast<BoolType>(l || r)); },
            [this](UnsignedNumberType &l, BoolType &r){ return Value(static_cast<BoolType>(l || r)); },

            [this](NumberType &l, UnsignedNumberType &r){ return Value(static_cast<BoolType>(l || r)); },
            [this](NumberType &l, NumberType &r){ return Value(static_cast<BoolType>(l || r)); },
            [this](NumberType &l, BoolType &r){ return Value(static_cast<BoolType>(l || r)); },

            [this](BoolType &l, UnsignedNumberType &r){ return Value(static_cast<BoolType>(l || r)); },
            [this](BoolType &l, NumberType &r){ return Value(static_cast<BoolType>(l || r)); },
            [this](BoolType &l, BoolType &r){ return Value(l || r); },

            [this](auto &l, auto &r){ return Value(VoidType()); }
    }, leftValue, rightValue);

    return result;
}

Value NotExpression::evaluate(Environment &environment) {
    if (!factorExpression) {
        return Value(VoidType());
    }
    Value value = factorExpression->evaluate(environment);

    Value result = std::visit(overload {
            [this](BoolType &val) { return Value(!val); },

            [this](auto &val){ return Value(VoidType()); }
    }, value);

    return result;
}
