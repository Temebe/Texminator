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
    return 0;
}
