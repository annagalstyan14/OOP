#include <iostream>
#include <sstream>
#include "Tokenizer.h"
#include "Parser.h"

Token nextToken(std::istringstream& iss);

void executeParsed(const ParsedCommand& pc) {
    std::cout << "EXECUTE: " << pc.action << "\n";
    if (!pc.positionals.empty()) {
        std::cout << "  positionals:\n";
        for (auto& p : pc.positionals) std::cout << "    " << p << "\n";
    }
    if (!pc.options.empty()) {
        std::cout << "  options:\n";
        for (auto& kv : pc.options) std::cout << "    --" << kv.first << " = '" << kv.second << "'\n";
    }
    std::cout << "----\n";
}

int main() {
    std::string line;
    std::cout << "ppt-cli (type 'exit' to quit)\n";
    while (true) {
        std::cout << "ppt-cli> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit" || line == "quit") break;

        std::istringstream strm(line);

        try {
            ParsedCommand pc = Parser::parse(strm);
            executeParsed(pc);
        }
        catch (const ParseError& e) {
            std::cerr << "Parse error: " << e.what() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
