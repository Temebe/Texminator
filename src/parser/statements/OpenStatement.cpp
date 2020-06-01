#include "parser/statements/OpenStatement.h"
#include "parser/expressions/VariableExpression.h"

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
    if (const auto identifier = dynamic_cast<VariableExpression*>(filePathExp.get())) {
        filePath = identifier->getName();
    } else {
        filePath = std::get<StringType>(castValue(filePathExp->evaluate(environment), STRING));
    }

    if (alias.empty()) {
        alias = filePath;
    }

    Value stream = std::make_shared<FileStream>(filePath, openMode);
    environment.addVariable(alias, stream);
}

const std::string &OpenStatement::getFilePath() const {
    return filePath;
}

const std::string &OpenStatement::getAlias() const {
    return alias;
}

const std::unique_ptr<Expression> &OpenStatement::getFilePathExp() const {
    return filePathExp;
}

OpenMode OpenStatement::getOpenMode() const {
    return openMode;
}
