#include <iostream>
#include <string>
#include "Fetch.h"
#include "Parser.h"
#include "Semantic.h"

void executeParsed(const CommandNode& node) {
    std::cout << "EXECUTE: " << node.cmd;
    if (!node.object.empty()) {
        std::cout << " " << node.object;
    }
    std::cout << "\n";
    if (!node.positionArgs.empty()) {
        std::cout << "  positionals:\n";
        for (const auto& p : node.positionArgs) {
            std::cout << "    " << p << "\n";
        }
    }
    if (!node.args.empty()) {
        std::cout << "  args:\n";
        for (const auto& a : node.args) {
            std::cout << "    " << a << "\n";
        }
    }
    if (!node.flags.empty()) {
        std::cout << "  options:\n";
        for (const auto& kv : node.flags) {
            std::cout << "    " << kv.first << " = '" << kv.second << "'\n";
        }
    }
    std::cout << "----\n";
}

int main() {
    Fetch fetch;
    std::string line;
    std::cout << "ppt-cli (type 'exit' to quit)\n";
    while (true) {
        line = fetch.getCommand(std::cin);
        if (line.empty() || line == "exit" || line == "quit") {
            break;
        }

        Tokenizer tz(line);
        Parser parser(tz);
        CommandNode node;
        std::string err;

        if (!parser.parseDFA(node, err)) {
            std::cerr << "Parse error: " << err << "\n";
            continue;
        }

        SemanticAnalyzer analyzer;
        SemanticResult result = analyzer.analyze(node);
        if (!result.ok) {
            std::cerr << "Semantic error: " << result.message << "\n";
            continue;
        }

        executeParsed(node);
    }
    return 0;
}