#include "Parser/Parser.h"
#include "Semantic/Semantic.h"
#include "Parser/CommandFactory.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

int main() {
    std::cout << "ppt-cli parser test (type 'exit' to quit)\n> ";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty() || line == "exit" || line == "quit") break;

        std::istringstream iss(line);
        ppt_cli::Parser parser(iss);
        auto command = parser.parse();

        if (!command) {
            std::cerr << "❌ Parse error: " << parser.getError() << "\n";
        } else {
            std::cout << "✅ Parsed OK\n";
            
            // Semantic (simplified)
            std::vector<std::unique_ptr<ppt_cli::ICommand>> commands;
            commands.push_back(std::move(command));
            
            try {
                ppt_cli::SemanticAnalyzer analyzer(commands);
                analyzer.analyze();
                std::cout << "✅ Semantic OK\n";
            } catch (const std::exception& e) {
                std::cerr << "❌ " << e.what() << "\n";
            }
        }
        std::cout << "> ";
    }
    return 0;
}