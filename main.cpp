#include "Tokenizer.h"
#include "Parser.h"
#include "Fetcher.h"
#include <iostream>
#include <string>

int main() {
    Fetcher fetcher;

    std::string input;
    std::cout << "ppt-cli> ";
    while (std::getline(std::cin, input)) {
        if (input == "exit" || input == "quit") break;

        auto tokens = Tokenizer::tokenize(input);
        command cmd = Parser::parse(tokens);
        fetcher.fetch(cmd);

        std::cout << "ppt-cli> ";
    }

    return 0;
}