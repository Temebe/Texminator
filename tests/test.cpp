#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <catch2/catch_reporter_teamcity.hpp>
#include <parser/expressions/IncrementCharExpression.h>
#include "Scanner.h"
#include "parser/Parser.h"
#include "StringSource.h"
#include "parser/statements/OpenStatement.h"
#include "parser/statements/BreakStatement.h"
#include "parser/statements/ContinueStatement.h"
#include "parser/statements/ReturnStatement.h"
#include "parser/statements/AliasDeclarationStatement.h"
#include "parser/statements/BlockStatement.h"
#include "parser/statements/IdentifierExpressionStatement.h"
#include "parser/statements/ForStatement.h"
#include "parser/statements/FunctionDeclarationStatement.h"
#include "parser/statements/IfMatchesStatement.h"
#include "parser/statements/IfStatement.h"
#include "parser/statements/VariableDeclarationStatement.h"
#include "parser/statements/WriteStatement.h"
#include "parser/statements/ExpressionStatement.h"
#include "parser/expressions/ComparisonExpressions.h"
#include "parser/expressions/LiteralExpression.h"
#include "parser/expressions/ArithmeticExpressions.h"
#include "parser/expressions/BracketExpression.h"

#include "TexminatorExceptions.h"

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

void executeCode(const std::string& code_, Environment& env_) {
    Parser parser;
    std::unique_ptr<Source> source = std::make_unique<StringSource>(code_);
    Scanner scanner(std::move(source));

    while (auto statement = parser.parseStatement(scanner)) {
        statement->execute(env_);
    }
}

std::unique_ptr<Expression> parseCompoundExpression(const std::string &code_) {
    Parser parser;
    std::unique_ptr<Source> source = std::make_unique<StringSource>(code_);
    Scanner scanner(std::move(source));

    std::unique_ptr<Expression> result;
    result = parser.parseCompoundExpression(scanner);

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

    CHECK(checkStatement<IdentifierExpressionStatement>("arg += 3;"));
    CHECK(checkStatement<IdentifierExpressionStatement>("var = (2*(3-(4/5)));"));
    CHECK(checkStatement<IdentifierExpressionStatement>("h -= 4;"));
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

TEST_CASE("Compound expression and order of operations") {
    Environment env;

    SECTION("1 + 3 > 2 + 3 * 4") {
        auto exp = parseCompoundExpression("1 + 3 > 2 + 3 * 4;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == false);
    }

    SECTION("2 + 2 * 2") {
        auto exp = parseCompoundExpression("2 + 2 * 2;");

        REQUIRE(exp);
        CHECK(std::get<UnsignedNumberType>(exp->evaluate(env)) == 6);
    }

    SECTION("(true || false) && true") {
        auto exp = parseCompoundExpression("(true || false) && true;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == true);
    }

    SECTION("(true || false) && false") {
        auto exp = parseCompoundExpression("(true || false) && false;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == false);
    }

    SECTION("(false || false) && true") {
        auto exp = parseCompoundExpression("(false || false) && true;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == false);
    }

    SECTION("!false") {
        auto exp = parseCompoundExpression("!false;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == true);
    }

    SECTION("23 + 7243234 + 23312 * 31 != 1") {
        auto exp = parseCompoundExpression("23 + 7243234 + 23312 * 31 != 1;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == false);
    }

    SECTION("0 == 0") {
        auto exp = parseCompoundExpression("0 == 0;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == true);
    }

    SECTION("2 > 3") {
        auto exp = parseCompoundExpression("2 > 3;");

        REQUIRE(exp);
        CHECK(std::get<BoolType>(exp->evaluate(env)) == false);
    }

}

TEST_CASE("Variable declaration statement") {
    Environment env;
    env.createNewScope(local);

    SECTION("Number type, ex A") {
        auto statement = parseStatement<VariableDeclarationStatement>("number x = 3;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 3);
    }

    SECTION("Number type, ex B") {
        auto statement = parseStatement<VariableDeclarationStatement>("number x = -53;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == -53);
    }

    SECTION("Unsigned number type, ex A") {
        auto statement = parseStatement<VariableDeclarationStatement>("unsigned x = 53;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<UnsignedNumberType>(variable.value()) == 53);
    }

    SECTION("Unsigned number type, ex B") {
        auto statement = parseStatement<VariableDeclarationStatement>("unsigned number x;");
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<UnsignedNumberType>(variable.value()) == 0);
    }

    SECTION("Bool type, ex A") {
        auto statement = parseStatement<VariableDeclarationStatement>("bool x = true;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<BoolType>(variable.value()) == true);
    }

    SECTION("Float type, ex A") {
        auto statement = parseStatement<VariableDeclarationStatement>("float x = 4.5;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<FloatType>(variable.value()) == 4.5);
    }

    SECTION("String type, ex A") {
        auto statement = parseStatement<VariableDeclarationStatement>("string x = \"test value\";");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<StringType>(variable.value()) == "test value");
    }

    SECTION("String type, ex B") {
        auto statement = parseStatement<VariableDeclarationStatement>("string x = 4.5;");
        REQUIRE(statement);
        REQUIRE_THROWS_AS(statement->execute(env), BadCastException);
    }

    SECTION("Char type, ex A") {
        auto statement = parseStatement<VariableDeclarationStatement>("char x = \"3\";");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<CharType>(variable.value()) == '3');
    }

    env.destroyCurrentScope();

}

TEST_CASE("Expression statements") {
    Environment env;
    env.createNewScope(local);

    SECTION("+= with integer") {
        env.addVariable("x", Value(NumberType(-3)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x += 2;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == -1);
    }

    SECTION("+= with string and char A") {
        env.addVariable("x", Value(std::string("1234")));
        auto statement = parseStatement<IdentifierExpressionStatement>("x += \"5\";");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<StringType>(variable.value()) == "12345");
    }

    SECTION("+= with string and char B") {
        env.addVariable("x", StringType(""));
        env.addVariable("y", CharType('1'));
        auto statement = parseStatement<IdentifierExpressionStatement>("x += y + \"2345\";");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<StringType>(variable.value()) == "12345");
    }

    SECTION("+= with two strings") {
        env.addVariable("x", Value(std::string("12")));
        auto statement = parseStatement<IdentifierExpressionStatement>("x += \"345\";");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<StringType>(variable.value()) == "12345");
    }

    SECTION("-=, example A") {
        env.addVariable("x", Value(NumberType(-3)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x -= 5;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == -8);
    }

    SECTION("-=, example B") {
        env.addVariable("x", Value(NumberType(-3)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x -= -5;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 2);
    }

    SECTION("-=, example C") {
        env.addVariable("x", Value(FloatType(3.5)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x -= 2.25;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<FloatType>(variable.value()) == 1.25);
    }

    SECTION("/=, example A") {
        env.addVariable("x", Value(NumberType(8)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x /= 2;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 4);
    }

    SECTION("/=, example B") {
        env.addVariable("x", Value(NumberType(5)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x /= 1.25;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 4);
    }

    SECTION("*=, example A") {
        env.addVariable("x", Value(NumberType(4)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x *= 2.25;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 9);
    }

    SECTION("*=, example B") {
        env.addVariable("x", Value(FloatType(1.5)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x *= 2.25;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<FloatType>(variable.value()) == 3.375);
    }

    SECTION("=, example A") {
        env.addVariable("x", Value(FloatType(1.5)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x = 2.25;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<FloatType>(variable.value()) == 2.25);
    }

    SECTION("=, example B") {
        env.addVariable("x", Value(FloatType(1.5)));
        auto statement = parseStatement<IdentifierExpressionStatement>("x = 5;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<FloatType>(variable.value()) == 5);
    }

    env.destroyCurrentScope();
}

TEST_CASE("If statements") {
    Environment env;
    env.createNewScope(local);

    SECTION("Example A") {
        env.addVariable("x", Value(NumberType(0)));
        auto statement = parseStatement<IfStatement>("if(true) x += 1;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 1);
    }

    SECTION("Example B") {
        env.addVariable("x", Value(NumberType(0)));
        auto statement = parseStatement<IfStatement>("if(3 > 4) x = 3; else x = 5;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 5);
    }



    env.destroyCurrentScope();
}

TEST_CASE("BlockStatements") {
    Environment env;
    env.createNewScope(local);

    SECTION("Changing variable from local scope one level up") {
        env.addVariable(("x"), Value(NumberType(0)));
        auto statement = parseStatement<BlockStatement>(
                "{"
                "   x = 3;"
                "   x += 1;"
                "}");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 4);
    }

    SECTION("Variable created in scope below is not accessible from scope below") {
        env.addVariable(("x"), Value(NumberType(0)));
        auto statement = parseStatement<BlockStatement>(
                "{"
                "   number y = 1;"
                "   x = 3;"
                "}");
        REQUIRE(statement);
        statement->execute(env);

        auto variableX = env.getVariable("x");
        REQUIRE(variableX);
        CHECK(std::get<NumberType>(variableX.value()) == 3);
        REQUIRE_THROWS(env.getVariable("y"));
    }

    SECTION("Statements after break do not execute") {
        env.addVariable(("x"), Value(NumberType(0)));
        auto statement = parseStatement<BlockStatement>(
                "{"
                "   x = 3;"
                "   break;"
                "   x += 1;"
                "}");
        REQUIRE(statement);
        statement->execute(env);
        CHECK(statement->getExecStatus() == broke);

        auto variable = env.getVariable("x");
        REQUIRE(variable);
        CHECK(std::get<NumberType>(variable.value()) == 3);
    }

    env.destroyCurrentScope();

}

TEST_CASE("File operations tests") {
    Environment env;
    env.createNewScope(local);
    // prepare text file
    std::ofstream exampleFile("example.txt");
    exampleFile << "test 1234\n";
    exampleFile << "5678";
    exampleFile.close();

    SECTION("Open and read line") {
        auto statement = parseStatement<OpenStatement>("open example.txt;");
        REQUIRE(statement);

        statement->execute(env);
        auto variable = env.getVariable("example.txt");
        REQUIRE(variable);
        CHECK_NOTHROW(std::get<StreamType>(variable.value()));

        auto stream = std::get<StreamType>(variable.value());
        CHECK(stream->readLine() == "test 1234");
        CHECK(stream->readLine() == "5678");
    }

    SECTION("Open and read two characters") {
        auto statement = parseStatement<OpenStatement>("open example.txt;");
        REQUIRE(statement);

        statement->execute(env);
        auto variable = env.getVariable("example.txt");
        REQUIRE(variable);
        CHECK_NOTHROW(std::get<StreamType>(variable.value()));

        auto stream = std::get<StreamType>(variable.value());
        CHECK(stream->readChar() == 't');
        CHECK(stream->readChar() == 'e');
    }

    SECTION("Open, increment character and then read") {
        auto statement = parseStatement<OpenStatement>("open example.txt;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("example.txt");
        REQUIRE(variable);
        CHECK_NOTHROW(std::get<StreamType>(variable.value()));

        auto expStatement = parseStatement<ExpressionStatement>("example.txt+;");
        REQUIRE(expStatement);
        expStatement->execute(env);

        auto stream = std::get<StreamType>(variable.value());
        CHECK(stream->readChar() == 'e');
    }

    SECTION("Open, increment line and then read line") {
        auto statement = parseStatement<OpenStatement>("open example.txt;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("example.txt");
        REQUIRE(variable);
        CHECK_NOTHROW(std::get<StreamType>(variable.value()));

        auto expStatement = parseStatement<ExpressionStatement>("example.txt++;");
        REQUIRE(expStatement);
        expStatement->execute(env);

        auto stream = std::get<StreamType>(variable.value());
        CHECK(stream->readLine() == "5678");
    }

    SECTION("Open, increment line and then read character and then read line") {
        auto statement = parseStatement<OpenStatement>("open example.txt;");
        REQUIRE(statement);
        statement->execute(env);

        auto variable = env.getVariable("example.txt");
        REQUIRE(variable);
        CHECK_NOTHROW(std::get<StreamType>(variable.value()));

        auto expStatement = parseStatement<ExpressionStatement>("example.txt++;");
        REQUIRE(expStatement);
        expStatement->execute(env);

        auto stream = std::get<StreamType>(variable.value());
        CHECK(stream->readChar() == '5');
        CHECK(stream->readLine() == "678");
    }

    SECTION("Write statement, simple example") {
        executeCode(
                "open example.txt;"
                "example.txt -> \"beta\";", env);

        auto variable = env.getVariable("example.txt");
        REQUIRE(variable);
        CHECK_NOTHROW(std::get<StreamType>(variable.value()));

        auto stream = std::get<StreamType>(variable.value());
        stream->close();

        std::ifstream file("example.txt");
        std::string result;
        std::getline(file, result);
        CHECK(result == "beta 1234");
    }

    SECTION("Write statement with formatted string") {
        executeCode(
                "open example.txt;"
                "example.txt -> \"beta{}-{}\".(\"13345\", \"25545\");", env);

        auto variable = env.getVariable("example.txt");
        REQUIRE(variable);
        CHECK_NOTHROW(std::get<StreamType>(variable.value()));

        auto stream = std::get<StreamType>(variable.value());
        stream->close();

        std::ifstream file("example.txt");
        std::string result;
        std::getline(file, result);
        CHECK(result == "beta13345-25545");
    }

    env.destroyCurrentScope();
}
