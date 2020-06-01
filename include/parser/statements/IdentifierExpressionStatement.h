#ifndef TEXMINATOR_IDENTIFIEREXPRESSIONSTATEMENT_H
#define TEXMINATOR_IDENTIFIEREXPRESSIONSTATEMENT_H

#include <Token.h>
#include <parser/expressions/Expression.h>
#include "Statement.h"

class IdentifierExpressionStatement : public Statement {
public:
    IdentifierExpressionStatement(std::string identifier_, TokenType operationType_,
                                  std::unique_ptr<Expression> expression_);
    void execute(Environment& environment) override;

private:
    static std::unique_ptr<Expression> createAdditionStatement(const Value& l_, const Value& r_,
                                                               bool subtraction = false);
    static std::unique_ptr<Expression> createSubtractionStatement(const Value& l_, const Value& r_);
    static std::unique_ptr<Expression> createMultiplicationStatement(const Value& l_, const Value& r_,
                                                                     bool division = false);
    static std::unique_ptr<Expression> createDivisionStatement(const Value& l_, const Value& r_);

    const std::string identifier;
    const TokenType operationType;
    const std::unique_ptr<Expression> expression;
};


#endif //TEXMINATOR_IDENTIFIEREXPRESSIONSTATEMENT_H
