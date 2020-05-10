cmake_minimum_required(VERSION 3.15)
project(Texminator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Texminator src/main.cpp src/Scanner.cpp include/Scanner.h src/Source.cpp include/Source.h src/StandardSource.cpp include/StandardSource.h src/FileSource.cpp include/FileSource.h include/Token.h src/Token.cpp src/parser/Parser.cpp include/parser/Parser.h src/parser/Statement.cpp include/parser/Statement.h src/parser/Expression.cpp include/parser/Expression.h src/parser/Environment.cpp include/parser/Environment.h src/parser/Value.cpp include/parser/Value.h src/parser/Scope.cpp include/parser/Scope.h src/parser/BlockStatement.cpp include/parser/BlockStatement.h src/parser/Function.cpp include/parser/Function.h include/parser/ParserTraits.h src/parser/IfStatement.cpp include/parser/IfStatement.h src/parser/BreakStatement.cpp include/parser/BreakStatement.h src/parser/ContinueStatement.cpp include/parser/ContinueStatement.h src/parser/ReturnStatement.cpp include/parser/ReturnStatement.h include/parser/ParserException.h include/parser/FunctionBody.h)

target_include_directories(Texminator PRIVATE include)
