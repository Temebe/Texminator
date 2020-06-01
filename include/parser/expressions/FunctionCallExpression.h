#ifndef TEXMINATOR_FUNCTIONCALLEXPRESSION_H
#define TEXMINATOR_FUNCTIONCALLEXPRESSION_H


#include "Expression.h"

// change std::list to std::vector or deque
class FunctionCallExpression : public Expression {
public:
    FunctionCallExpression(std::string name_, std::list<std::unique_ptr<Expression>> arguments_);
    Value evaluate(Environment &environment) override ;

private:
    std::string name;
    std::list<std::unique_ptr<Expression>> arguments;
};


#endif //TEXMINATOR_FUNCTIONCALLEXPRESSION_H
