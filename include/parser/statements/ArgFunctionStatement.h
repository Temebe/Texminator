#ifndef TEXMINATOR_ARGFUNCTIONSTATEMENT_H
#define TEXMINATOR_ARGFUNCTIONSTATEMENT_H

#include <utility>
#include <vector>

#include "Statement.h"
#include "parser/Environment.h"

class ArgFunctionStatement : public Statement {
public:
    explicit ArgFunctionStatement(std::vector<std::string> args_) : arguments(std::move(args_)) {};
    void execute(Environment &environment) override;

    static const std::string argNumParam;

private:
    std::vector<std::string> arguments;

};


#endif //TEXMINATOR_ARGFUNCTIONSTATEMENT_H
