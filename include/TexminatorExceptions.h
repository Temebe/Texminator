#ifndef TEXMINATOR_TEXMINATOREXCEPTIONS_H
#define TEXMINATOR_TEXMINATOREXCEPTIONS_H

#include <string>

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

struct NonExistentVariableException : public TexminatorException {
    NonExistentVariableException(const std::string &identifier_)
        : TexminatorException(identifier_ + " variable does not exist") {}
};

struct EvaluationErrorException : public TexminatorException {
    explicit EvaluationErrorException(const std::string &msg_) : TexminatorException(msg_) {}
};

struct BadCastException : public TexminatorException {
    explicit BadCastException(const std::string &msg_) : TexminatorException(msg_) {}
};

struct BadStreamException : public TexminatorException {
    explicit BadStreamException(const std::string &msg_) : TexminatorException(msg_) {}
};

#endif //TEXMINATOR_TEXMINATOREXCEPTIONS_H
