cmake_minimum_required(VERSION 3.15)
project(Texminator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Texminator main.cpp Scanner.cpp Scanner.h Source.cpp Source.h StandardSource.cpp StandardSource.h FileSource.cpp FileSource.h Token.h)