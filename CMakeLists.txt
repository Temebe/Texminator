cmake_minimum_required(VERSION 3.15)
project(Texminator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Texminator src/main.cpp src/Scanner.cpp include/Scanner.h src/Source.cpp include/Source.h src/StandardSource.cpp include/StandardSource.h src/FileSource.cpp include/FileSource.h include/Token.h src/Token.cpp src/parser/Parser.cpp include/parser/Parser.h src/parser/Statement.cpp include/parser/Statement.h src/parser/Expression.cpp include/parser/Expression.h src/parser/Environment.cpp include/parser/Environment.h src/parser/Value.cpp include/parser/Value.h src/parser/Scope.cpp include/parser/Scope.h)