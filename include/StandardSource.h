#ifndef TEXMINATOR_STANDARDSOURCE_H
#define TEXMINATOR_STANDARDSOURCE_H


#include "Source.h"

class StandardSource : public Source {
public:
    StandardSource();
    void goNext() override;
};


#endif //TEXMINATOR_STANDARDSOURCE_H
