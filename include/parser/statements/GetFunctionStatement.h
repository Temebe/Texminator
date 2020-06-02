#ifndef TEXMINATOR_GETFUNCTIONSTATEMENT_H
#define TEXMINATOR_GETFUNCTIONSTATEMENT_H

#include "Statement.h"
#include "parser/Environment.h"

class GetFunctionStatement : public Statement {
public:
    void execute(Environment &environment) override;

    static const std::string toMatchParameterName;
    static const std::string regexParameterName;
};


#endif //TEXMINATOR_GETFUNCTIONSTATEMENT_H
