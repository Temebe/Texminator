cmake_minimum_required(VERSION 3.15)
project(Texminator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Texminator src/main.cpp src/Scanner.cpp include/Scanner.h src/Source.cpp include/Source.h src/StandardSource.cpp include/StandardSource.h src/FileSource.cpp include/FileSource.h include/Token.h src/Token.cpp src/Parser.cpp include/Parser.h src/Statement.cpp include/Statement.h src/Expression.cpp include/Expression.h src/Environment.cpp include/Environment.h src/Value.cpp include/Value.h src/Scope.cpp include/Scope.h)