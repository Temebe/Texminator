#include "parser/statements/OpenStatement.h"

#include <utility>

OpenStatement::OpenStatement(std::string filePath_, std::string alias_, OpenMode mode_)
        : filePath(std::move(filePath_)),
          alias(std::move(alias_)),
          openMode(mode_) {}

OpenStatement::OpenStatement(std::unique_ptr<Expression> exp_, std::string alias_, OpenMode mode_)
        : filePathExp(std::move(exp_)),
          alias(std::move(alias_)),
          openMode(mode_) {}

void OpenStatement::execute(Environment &environment) {

}
