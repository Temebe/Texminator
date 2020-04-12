#ifndef TEXMINATOR_STANDARDSOURCE_H
#define TEXMINATOR_STANDARDSOURCE_H


#include "Source.h"

class StandardSource : public Source {
public:
    StandardSource();
    char getChar() override;
    void next() override;
};


#endif //TEXMINATOR_STANDARDSOURCE_H
