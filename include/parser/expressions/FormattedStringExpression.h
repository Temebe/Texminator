#ifndef TEXMINATOR_FORMATTEDSTRINGEXPRESSION_H
#define TEXMINATOR_FORMATTEDSTRINGEXPRESSION_H


#include "Expression.h"

class FormattedStringExpression : public Expression {
public:
    explicit FormattedStringExpression(std::unique_ptr<Expression> string_,
                                       std::list<std::unique_ptr<Expression>> arguments_);
    Value evaluate(Environment &environment) override;

private:
    const std::unique_ptr<Expression> stringExpression;
    const std::list<std::unique_ptr<Expression>> arguments;
};


#endif //TEXMINATOR_FORMATTEDSTRINGEXPRESSION_H
