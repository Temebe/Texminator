#include "parser/statements/AliasDeclarationStatement.h"

#include <utility>

void AliasDeclarationStatement::execute(Environment &environment) {

}

AliasDeclarationStatement::AliasDeclarationStatement(std::string alias_, std::string underlying_)
    : alias(std::move(alias_)),
      underlyingVariable(std::move(underlying_)) {}
