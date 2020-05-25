/**
 * @file StringSource.h
 *
 * @brief Class wrapping string as a source
 *
 * This class is mainly created with testing on mind. We simply provide a string with source code and this class
 * just moves character by character reading from it. As memory should not be that much of a concern
 * it won't affect string in any way ant just move with iterator and not delete what was already read.
 */

#ifndef TEXMINATOR_STRINGSOURCE_H
#define TEXMINATOR_STRINGSOURCE_H

#include <Source.h>

#include <string>


class StringSource : public Source {
public:
    explicit StringSource(std::string code_);
    void goNext() override;

private:
    const std::string code;
    std::string::const_iterator codeIt;
};


#endif //TEXMINATOR_STRINGSOURCE_H
