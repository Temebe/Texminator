#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

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


static inline std::ostream& operator<< (std::ostream& os, const Value& val)
{
    os << "val";
    return os;
}

namespace boost::test_tools::tt_detail {
            std::ostream& operator<<(std::ostream& os, Value const& ts)
            {
                return ::operator<<(os, ts);
            }
        }


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


BOOST_AUTO_TEST_CASE(test_simplechecks) {
    BOOST_TEST(checkStatement<OpenStatement>("open to write file.txt;"));
    BOOST_TEST(checkStatement<OpenStatement>("open to read some_file;"));
    BOOST_TEST(checkStatement<OpenStatement>("open to write \"../file.txt\";"));
    BOOST_TEST(checkStatement<OpenStatement>("open to read \"../fil22e.txt\";"));
    BOOST_TEST(checkStatement<OpenStatement>("open to write file.txt as exe;"));
    BOOST_TEST(checkStatement<OpenStatement>("open to read some_file as fil;"));
    BOOST_TEST(checkStatement<OpenStatement>("open to write \"../file.txt\" as file2;"));
    BOOST_TEST(checkStatement<OpenStatement>("open to read \"../fil22e.txt\" as other_file;"));
    BOOST_TEST(checkStatement<OpenStatement>("open file.txt;"));
    BOOST_TEST(checkStatement<OpenStatement>("open file.txt as file;"));
    BOOST_TEST(checkStatement<OpenStatement>("open \"file.txt\";"));
    BOOST_TEST(checkStatement<OpenStatement>("open \"file.txt as file\";"));

    BOOST_TEST(!checkStatement<OpenStatement>("open to write file.txt"));
    BOOST_TEST(!checkStatement<OpenStatement>("open"));
    BOOST_TEST(!checkStatement<OpenStatement>("open as to when off write file.txt;"));

    BOOST_TEST(checkStatement<BreakStatement>("break;"));
    BOOST_TEST(checkStatement<BreakStatement>(" break  ;"));

    BOOST_TEST(checkStatement<ContinueStatement>("continue;"));
    BOOST_TEST(!checkStatement<ContinueStatement>("break;"));
    BOOST_TEST(!checkStatement<ContinueStatement>(";"));

    BOOST_TEST(checkStatement<ReturnStatement>("return;"));
    BOOST_TEST(checkStatement<ReturnStatement>("return 5;"));
    BOOST_TEST(checkStatement<ReturnStatement>("return \"asdfgh\";"));
    BOOST_TEST(!checkStatement<ReturnStatement>(";"));
    BOOST_TEST(!checkStatement<ReturnStatement>("continue;"));

    BOOST_TEST(checkStatement<ExpressionStatement>("arg += 3;"));
    BOOST_TEST(checkStatement<ExpressionStatement>("var = (2*(3-(4/5)));"));
    BOOST_TEST(checkStatement<ExpressionStatement>("h -= 4;"));

}

BOOST_AUTO_TEST_CASE(test_open_statement_A) {
    Environment env;
    std::unique_ptr<OpenStatement> statement = parseStatement<OpenStatement>("open to write \"file.txt\";");
    BOOST_TEST(statement.get() != nullptr);
    BOOST_TEST(statement->getOpenMode() == writeMode);
    BOOST_TEST(statement->getAlias().empty());

    auto path = statement->getFilePathExp()->evaluate(env).getString();
    BOOST_TEST(path.value_or("") == "file.txt");
}

BOOST_AUTO_TEST_CASE(test_open_statement_B) {
    std::unique_ptr<OpenStatement> statement = parseStatement<OpenStatement>("open file2.txt;");
    BOOST_TEST(statement.get() != nullptr);
    BOOST_TEST(statement->getOpenMode() == readwrite);
    BOOST_TEST(statement->getAlias().empty());
}

BOOST_AUTO_TEST_CASE(test_open_statement_C) {
    std::unique_ptr<OpenStatement> statement = parseStatement<OpenStatement>("open to read file.txt as sth;");
    BOOST_TEST(statement.get() != nullptr);
    BOOST_TEST(statement->getOpenMode() == readMode);
    BOOST_TEST(statement->getAlias() == "sth");
}
