#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <catch2/catch_reporter_teamcity.hpp>
#include "Scanner.h"
#include "parser/Parser.h"
#include "StringSource.h"
#include "parser/statements/OpenStatement.h"
#include "parser/statements/BreakStatement.h"
#include "parser/statements/ContinueStatement.h"
#include "parser/statements/ReturnStatement.h"
#include "parser/statements/AliasDeclarationStatement.h"
#include "parser/statements/BlockStatement.h"
#include "parser/statements/ExpressionStatement.h"
#include "parser/statements/ForStatement.h"
#include "parser/statements/FunctionDeclarationStatement.h"
#include "parser/statements/IfMatchesStatement.h"
#include "parser/statements/VariableDeclarationStatement.h"
#include "parser/statements/WriteStatement.h"

template<typename T>
bool checkStatement(const std::string& code_) {
    Parser parser;
    std::unique_ptr<Source> source = std::make_unique<StringSource>(code_);
    Scanner scanner(std::move(source));

    return dynamic_cast<T *>(parser.parseStatement(scanner).get()) != nullptr;
}

template<typename T>
std::unique_ptr<T> parseStatement(const std::string& code_) {
    Parser parser;
    std::unique_ptr<Source> source = std::make_unique<StringSource>(code_);
    Scanner scanner(std::move(source));

    std::unique_ptr<T> result;
    auto statement = parser.parseStatement(scanner);

    if (dynamic_cast<T*>(statement.get())) { // Check if statement even parsed properly to given T type
        result.reset(dynamic_cast<T*>(statement.release()));
    }
    return std::move(result);
}

TEST_CASE("Simple parsing checks") {
    REQUIRE(checkStatement<OpenStatement>("open to write file.txt;"));
    REQUIRE(checkStatement<OpenStatement>("open to read some_file;"));
    REQUIRE(checkStatement<OpenStatement>("open to write \"../file.txt\";"));
    REQUIRE(checkStatement<OpenStatement>("open to read \"../fil22e.txt\";"));
    REQUIRE(checkStatement<OpenStatement>("open to write file.txt as exe;"));
    REQUIRE(checkStatement<OpenStatement>("open to read some_file as fil;"));
    REQUIRE(checkStatement<OpenStatement>("open to write \"../file.txt\" as file2;"));
    REQUIRE(checkStatement<OpenStatement>("open to read \"../fil22e.txt\" as other_file;"));
    REQUIRE(checkStatement<OpenStatement>("open file.txt;"));
    REQUIRE(checkStatement<OpenStatement>("open file.txt as file;"));
    REQUIRE(checkStatement<OpenStatement>("open \"file.txt\";"));
    REQUIRE(checkStatement<OpenStatement>("open \"file.txt as file\";"));

    REQUIRE_FALSE(checkStatement<OpenStatement>("open to write file.txt"));
    REQUIRE_FALSE(checkStatement<OpenStatement>("open"));
    REQUIRE_FALSE(checkStatement<OpenStatement>("open as to when off write file.txt;"));

    REQUIRE(checkStatement<BreakStatement>("break;"));
    REQUIRE(checkStatement<BreakStatement>(" break  ;"));

    REQUIRE(checkStatement<ContinueStatement>("continue;"));
    REQUIRE_FALSE(checkStatement<ContinueStatement>("break;"));
    REQUIRE_FALSE(checkStatement<ContinueStatement>(";"));

    REQUIRE(checkStatement<ReturnStatement>("return;"));
    REQUIRE(checkStatement<ReturnStatement>("return 5;"));
    REQUIRE(checkStatement<ReturnStatement>("return \"asdfgh\";"));
    REQUIRE_FALSE(checkStatement<ReturnStatement>(";"));
    REQUIRE_FALSE(checkStatement<ReturnStatement>("continue;"));

    REQUIRE(checkStatement<ExpressionStatement>("arg += 3;"));
    REQUIRE(checkStatement<ExpressionStatement>("var = (2*(3-(4/5)));"));
    REQUIRE(checkStatement<ExpressionStatement>("h -= 4;"));
}

TEST_CASE("Parsing open statements") {
    Environment env;
    std::unique_ptr<OpenStatement> statement;

    SECTION("Example A") {
        statement = parseStatement<OpenStatement>("open to write \"file.txt\";");
        REQUIRE(statement.get() != nullptr);
        REQUIRE(statement->getOpenMode() == writeMode);
        REQUIRE(statement->getAlias().empty());

        auto path = statement->getFilePathExp()->evaluate(env).getString();
        REQUIRE(path.value_or("") == "file.txt");
    }

    SECTION("Example B") {
        statement = parseStatement<OpenStatement>("open file2.txt;");
        REQUIRE(statement.get() != nullptr);
        REQUIRE(statement->getOpenMode() == readwrite);
        REQUIRE(statement->getAlias().empty());
    }

    SECTION("Example C") {
        statement = parseStatement<OpenStatement>("open to read file.txt as sth;");
        REQUIRE(statement.get() != nullptr);
        REQUIRE(statement->getOpenMode() == readMode);
        REQUIRE(statement->getAlias() == "sth");
    }

}
