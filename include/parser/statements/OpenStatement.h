#ifndef TEXMINATOR_OPENSTATEMENT_H
#define TEXMINATOR_OPENSTATEMENT_H

#include <parser/expressions/Expression.h>
#include "Statement.h"
#include "parser/Stream.h"

class OpenStatement : public Statement {
public:
    explicit OpenStatement(std::unique_ptr<Expression> exp_, std::string  alias_ = "", OpenMode mode_ = readwrite);
    explicit OpenStatement(std::string  filePath_, std::string  alias_ = "", OpenMode mode_ = readwrite);
    void execute(Environment& environment) override;

    [[nodiscard]] const std::string &getFilePath() const;
    [[nodiscard]] const std::string &getAlias() const;
    [[nodiscard]] const std::unique_ptr<Expression> &getFilePathExp() const;
    [[nodiscard]] OpenMode getOpenMode() const;

private:
    std::string filePath;

private:
    std::string alias;
    std::unique_ptr<Expression> filePathExp;
    OpenMode openMode;

};


#endif //TEXMINATOR_OPENSTATEMENT_H
