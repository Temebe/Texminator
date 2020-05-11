#ifndef TEXMINATOR_EXPRESSION_H
#define TEXMINATOR_EXPRESSION_H

#include "parser/Environment.h"
#include "parser/Value.h"

class Expression {
public:
    virtual Value evaluate(Environment &environment) = 0;
};


#endif //TEXMINATOR_EXPRESSION_H
