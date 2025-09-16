#pragma once
#include "Parser.h"
#include <iostream>

class Fetcher {
public:
 
    void fetch(const command& cmd) {
        if (cmd.type == commandType::UNKNOWN) {
            std::cout << "Error: Unknown command: " << cmd.action << std::endl;
            return;
        }
        std::cout << "Fetching Command: " << cmd.action << "\n";
        if (!cmd.args.empty()) {
            std::cout << "Arguments/options:\n";
            for (const auto& arg : cmd.args) {
                std::cout << "  " << arg << "\n";
            }
        }
        std::cout << "----\n";
    }
};