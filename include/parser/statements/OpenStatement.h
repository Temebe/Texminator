#ifndef TEXMINATOR_OPENSTATEMENT_H
#define TEXMINATOR_OPENSTATEMENT_H

#include <parser/expressions/Expression.h>
#include "Statement.h"

enum OpenMode {
    read, write, readwrite
};

class OpenStatement : public Statement {
public:
    explicit OpenStatement(std::unique_ptr<Expression> exp_, std::string  alias_ = "", OpenMode mode_ = readwrite);
    explicit OpenStatement(std::string  filePath_, std::string  alias_ = "", OpenMode mode_ = readwrite);
    void execute(Environment& environment) override;

private:
    std::string filePath;
    std::string alias;
    std::unique_ptr<Expression> filePathExp;
    OpenMode openMode;

};


#endif //TEXMINATOR_OPENSTATEMENT_H
