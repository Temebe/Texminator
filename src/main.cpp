#include <iostream>
#include <memory>

#include "FileSource.h"
#include "StandardSource.h"
#include "Scanner.h"
#include "parser/Parser.h"

int main(int argc, char *argv[]) {
    std::unique_ptr<Source> source;

    if (argc < 2) { /* Use standard input as an source */
        source = std::make_unique<StandardSource>();

    } else { /* Use file provided as an argument as a source */
        try {
            source = std::make_unique<FileSource>(argv[1]);
        } catch (const FileException &e) {
            std::cerr << e.what();
            return -1;
        }
    }

    Scanner scanner(std::move(source));
    Parser parser;
    parser.parse(scanner);

//    Token token = scanner.getCurrentToken();
//    unsigned int line = 0;
//
//    while (token.type != unknown && token.type != fileEnd) {
//        while (token.line != line) {
//            std::cout << std::endl;
//            ++line;
//        }
//        std::cout << token.toString() << "(" << token.line << ":" << token.pos << ") ";
//        token = scanner.consume();
//    }
//
//    if (token.type == unknown) {
//        std::cout << "Texminator encountered token it does not understand!" << std::endl;
//    }
    return 0;
}
