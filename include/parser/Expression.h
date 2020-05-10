#ifndef TEXMINATOR_EXPRESSION_H
#define TEXMINATOR_EXPRESSION_H

#include "Environment.h"
#include "Value.h"

class Expression {
public:
    virtual Value evaluate(Environment &environment) = 0;
};


#endif //TEXMINATOR_EXPRESSION_H
