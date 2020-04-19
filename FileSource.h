#ifndef TEXMINATOR_FILESOURCE_H
#define TEXMINATOR_FILESOURCE_H


#include <string>
#include <fstream>

#include "Source.h"

struct FileException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Error while reading file";
    }
};

class FileSource : public Source {
public:
    explicit FileSource(const std::string& fileName_);
    void next() override;

private:
    std::ifstream file;
};


#endif //TEXMINATOR_FILESOURCE_H
