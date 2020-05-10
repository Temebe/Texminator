#ifndef TEXMINATOR_PARSEREXCEPTION_H
#define TEXMINATOR_PARSEREXCEPTION_H

#include <exception>
#include <string>
#include <utility>

struct ParserException : public std::exception {
    ParserException() : msg("Parser encountered an error") {};
    explicit ParserException(std::string msg_) : msg(std::move(msg_)) {};

    [[nodiscard]] const char * what() const noexcept override {
        return msg.c_str();
    }
    const std::string msg;
};

#endif //TEXMINATOR_PARSEREXCEPTION_H
