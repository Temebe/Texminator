#ifndef TEXMINATOR_EXPRESSION_H
#define TEXMINATOR_EXPRESSION_H

#include <utility>

#include "parser/Environment.h"
#include "parser/Value.h"

class Expression {
public:
    virtual ~Expression();
    virtual Value evaluate(Environment &environment) = 0;
};

class TwoFactorExpression {
public:
    void setLeftExpression(std::unique_ptr<Expression> exp_);
    void setRightExpression(std::unique_ptr<Expression> exp_);

    [[nodiscard]] const std::unique_ptr<Expression> &getLeftExpression() const;
    [[nodiscard]] const std::unique_ptr<Expression> &getRightExpression() const;

protected:
    std::unique_ptr<Expression> leftExpression;
    std::unique_ptr<Expression> rightExpression;
};

class OneFactorExpression {
public:
    void setFactorExpression(std::unique_ptr<Expression> exp_);

    [[nodiscard]] const std::unique_ptr<Expression> &getFactorExpression() const;

protected:
    std::unique_ptr<Expression> factorExpression;
};

struct TexminatorException : public std::exception {
    explicit TexminatorException(std::string  msg_) : msg(std::move(msg_)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};

struct VoidOperationException : public TexminatorException {
    VoidOperationException() : TexminatorException("Tried to use void as an value") {}
};

struct WrongTypeException : public TexminatorException {
    explicit WrongTypeException(const std::string &msg_) : TexminatorException(msg_) {}
};



#endif //TEXMINATOR_EXPRESSION_H
