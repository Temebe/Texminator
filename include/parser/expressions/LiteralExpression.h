#ifndef TEXMINATOR_LITERALEXPRESSION_H
#define TEXMINATOR_LITERALEXPRESSION_H


#include "Expression.h"
#include "Token.h"

class LiteralExpression : public Expression {
public:
    explicit LiteralExpression(const Token& token_);
    Value evaluate(Environment &environment) override;

private:
    void initializeString(const std::string& val_);
    void initializeFloat(const std::string& val_);
    void initializeNumber(const std::string& val_);

    Value val;
};



#endif //TEXMINATOR_LITERALEXPRESSION_H
