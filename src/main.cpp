#include <iostream>
#include <memory>

#include "FileSource.h"
#include "StandardSource.h"
#include "Scanner.h"
#include "parser/Parser.h"
#include <filesystem>

int main(int argc, char *argv[]) {
    std::unique_ptr<Source> source;

    if (argc < 2) {
        std::cerr << "Usage example: ./Texminator example1.txr arg1 arg2 arg3" << std::endl;
    }

    try {
        source = std::make_unique<FileSource>(argv[1]);
    } catch (const FileException &e) {
        std::cerr << e.what();
        return -1;
    }
    std::vector<std::string> arguments(argv + 1, argv + argc);

    Scanner scanner(std::move(source));
    Parser parser;
    parser.parse(scanner, arguments);
    return 0;
}
