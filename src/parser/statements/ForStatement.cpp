#include "parser/statements/ForStatement.h"

ForStatement::ForStatement(std::string itName_, std::string sourceName_, ForType type_,
                           std::unique_ptr<Statement> body_)
                           : iteratorName(std::move(itName_)),
                             sourceName(std::move(sourceName_)),
                             type(type_),
                             body(std::move(body_)) {}

void ForStatement::execute(Environment &environment) {

}


