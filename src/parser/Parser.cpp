#include "parser/Parser.h"

void Parser::parse(Scanner &scanner_) {
    environment.createNewScope(local);



    environment.destroyCurrentScope();
}
