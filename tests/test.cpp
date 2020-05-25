#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "Scanner.h"
#include "parser/Parser.h"
#include "StringSource.h"
#include "parser/statements/OpenStatement.h"


template<typename T>
bool checkStatement(const std::string& code_) {
    Parser parser;
    std::unique_ptr<Source> source = std::make_unique<StringSource>(code_);
    Scanner scanner(std::move(source));

    return dynamic_cast<T *>(parser.parseStatement(scanner).get()) != nullptr;
}


BOOST_AUTO_TEST_CASE(test_open_statement) {
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
}
