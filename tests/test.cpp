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
#include "parser/expressions/ComparisonExpressions.h"
#include "parser/expressions/LiteralExpression.h"
#include "parser/expressions/ArithmeticExpressions.h"
#include "parser/expressions/BracketExpression.h"

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
    CHECK(checkStatement<OpenStatement>("open to write file.txt;"));
    CHECK(checkStatement<OpenStatement>("open to read some_file;"));
    CHECK(checkStatement<OpenStatement>("open to write \"../file.txt\";"));
    CHECK(checkStatement<OpenStatement>("open to read \"../fil22e.txt\";"));
    CHECK(checkStatement<OpenStatement>("open to write file.txt as exe;"));
    CHECK(checkStatement<OpenStatement>("open to read some_file as fil;"));
    CHECK(checkStatement<OpenStatement>("open to write \"../file.txt\" as file2;"));
    CHECK(checkStatement<OpenStatement>("open to read \"../fil22e.txt\" as other_file;"));
    CHECK(checkStatement<OpenStatement>("open file.txt;"));
    CHECK(checkStatement<OpenStatement>("open file.txt as file;"));
    CHECK(checkStatement<OpenStatement>("open \"file.txt\";"));
    CHECK(checkStatement<OpenStatement>("open \"file.txt as file\";"));

    CHECK_FALSE(checkStatement<OpenStatement>("open to write file.txt"));
    CHECK_FALSE(checkStatement<OpenStatement>("open"));
    CHECK_FALSE(checkStatement<OpenStatement>("open as to when off write file.txt;"));

    CHECK(checkStatement<BreakStatement>("break;"));
    CHECK(checkStatement<BreakStatement>(" break  ;"));

    CHECK(checkStatement<ContinueStatement>("continue;"));
    CHECK_FALSE(checkStatement<ContinueStatement>("break;"));
    CHECK_FALSE(checkStatement<ContinueStatement>(";"));

    CHECK(checkStatement<ReturnStatement>("return;"));
    CHECK(checkStatement<ReturnStatement>("return 5;"));
    CHECK(checkStatement<ReturnStatement>("return \"asdfgh\";"));
    CHECK_FALSE(checkStatement<ReturnStatement>(";"));
    CHECK_FALSE(checkStatement<ReturnStatement>("continue;"));

    CHECK(checkStatement<ExpressionStatement>("arg += 3;"));
    CHECK(checkStatement<ExpressionStatement>("var = (2*(3-(4/5)));"));
    CHECK(checkStatement<ExpressionStatement>("h -= 4;"));
}

TEST_CASE("Parsing open statements") {
    Environment env;
    std::unique_ptr<OpenStatement> statement;

    SECTION("Example A") {
        statement = parseStatement<OpenStatement>("open to write \"file.txt\";");
        REQUIRE(statement.get() != nullptr);
        CHECK(statement->getOpenMode() == writeMode);
        CHECK(statement->getAlias().empty());

        auto path = statement->getFilePathExp()->evaluate(env);
        REQUIRE(std::holds_alternative<StringType>(path));
        REQUIRE(std::get<StringType>(path) == "file.txt");
    }

    SECTION("Example B") {
        statement = parseStatement<OpenStatement>("open file2.txt;");
        REQUIRE(statement.get() != nullptr);
        CHECK(statement->getOpenMode() == readwrite);
        CHECK(statement->getAlias().empty());
    }

    SECTION("Example C") {
        statement = parseStatement<OpenStatement>("open to read file.txt as sth;");
        REQUIRE(statement.get() != nullptr);
        CHECK(statement->getOpenMode() == readMode);
        CHECK(statement->getAlias() == "sth");
    }

}

TEST_CASE("Variable declaration statements") {
    Environment env;
    std::unique_ptr<VariableDeclarationStatement> statement;

    SECTION("Example A") {
        statement = parseStatement<VariableDeclarationStatement>("unsigned number x;");
        REQUIRE(statement);
        CHECK(statement->getName() == "x");
        CHECK_FALSE(statement->getAssignmentExpression());
        CHECK(statement->getType() == UNSIGNED_NUMBER);
    }

    SECTION("Example B") {
        statement = parseStatement<VariableDeclarationStatement>("unsigned x;");
        REQUIRE(statement);
        CHECK(statement->getName() == "x");
        CHECK_FALSE(statement->getAssignmentExpression());
        CHECK(statement->getType() == UNSIGNED_NUMBER);
    }

    SECTION("Example C") {
        statement = parseStatement<VariableDeclarationStatement>("string x = \"test\";");
        REQUIRE(statement);
        CHECK(statement->getName() == "x");
        REQUIRE(statement->getAssignmentExpression());
        auto val = statement->getAssignmentExpression()->evaluate(env);

        CHECK(std::get<StringType>(val) == "test");
        CHECK(statement->getType() == STRING);
    }

    SECTION("Check bool type") {
        statement = parseStatement<VariableDeclarationStatement>("bool x = true;");
        REQUIRE(statement);
        REQUIRE(statement->getAssignmentExpression());
        auto val = statement->getAssignmentExpression()->evaluate(env);

        CHECK(statement->getType() == BOOL);
        CHECK(std::get<BoolType>(val) == true);
    }

    SECTION("Check char type") {
        statement = parseStatement<VariableDeclarationStatement>("char x = \"c\";");
        REQUIRE(statement);
        REQUIRE(statement->getAssignmentExpression());
        auto val = statement->getAssignmentExpression()->evaluate(env);

        CHECK(statement->getType() == CHAR);
        CHECK(std::get<CharType>(val) == 'c');
    }

    SECTION("Check float type") {
        statement = parseStatement<VariableDeclarationStatement>("float x = 1.5;");
        REQUIRE(statement);
        REQUIRE(statement->getAssignmentExpression());
        auto val = statement->getAssignmentExpression()->evaluate(env);

        CHECK(statement->getType() == FLOAT);
        // 1.5 should be stored properly as an float, without an error
        // therefore it should be safe to check by ==
        CHECK(std::get<FloatType>(val) == 1.5);
    }

    SECTION("Check unsigned type") {
        statement = parseStatement<VariableDeclarationStatement>("unsigned x = 3;");
        REQUIRE(statement);
        REQUIRE(statement->getAssignmentExpression());
        auto val = statement->getAssignmentExpression()->evaluate(env);

        CHECK(statement->getType() == UNSIGNED_NUMBER);
        CHECK(std::get<UnsignedNumberType>(val) == 3);
    }

    SECTION("Check number type") {
        statement = parseStatement<VariableDeclarationStatement>("number x = -3;");
        REQUIRE(statement);
        REQUIRE(statement->getAssignmentExpression());
        auto val = statement->getAssignmentExpression()->evaluate(env);

        CHECK(statement->getType() == NUMBER);
        CHECK(std::get<NumberType>(val) == -3);
    }

    SECTION("Check string type") {
        statement = parseStatement<VariableDeclarationStatement>("string x;");
        REQUIRE(statement);
        CHECK(statement->getType() == STRING);
    }

}

TEST_CASE("Parsing compound expressions") {

    SECTION("Example A") {
        auto statement = parseStatement<VariableDeclarationStatement>("bool x = 1 + 3 > 2 + 3 * 4;");

        REQUIRE(statement != nullptr);
        REQUIRE(statement->getAssignmentExpression());
        const auto& exp = statement->getAssignmentExpression();

        const auto *relation = dynamic_cast<RelationalExpression*>(exp.get());
        REQUIRE(relation != nullptr);
        REQUIRE(relation->getLeftExpression());
        REQUIRE(relation->getRightExpression());

        const auto *leftAddition = dynamic_cast<AdditionExpression*>(relation->getLeftExpression().get());
        REQUIRE(leftAddition != nullptr);
        REQUIRE(dynamic_cast<LiteralExpression*>(leftAddition->getLeftExpression().get()));
        REQUIRE(dynamic_cast<LiteralExpression*>(leftAddition->getRightExpression().get()));

        const auto *rightAddition = dynamic_cast<AdditionExpression*>(relation->getRightExpression().get());
        REQUIRE(rightAddition != nullptr);
        REQUIRE(dynamic_cast<LiteralExpression*>(rightAddition->getLeftExpression().get()));
        REQUIRE(dynamic_cast<MultiplicationExpression*>(rightAddition->getRightExpression().get()));
    }
}
