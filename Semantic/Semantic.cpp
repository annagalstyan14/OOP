// Semantic/Semantic.cpp
#include "Semantic.h"
#include "../ICommand.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

SemanticAnalyzer::SemanticAnalyzer(const std::vector<std::unique_ptr<ICommand>>& commands)
    : commands_(commands) {}

void SemanticAnalyzer::analyze() {
    if (commands_.empty()) {
        semanticError("No commands to analyze");
    }
    std::cout << "[SEMANTIC] " << commands_.size() << " command(s) OK\n";
}

void SemanticAnalyzer::semanticError(const std::string& msg) {
    throw std::runtime_error("Semantic error: " + msg);
}

} // namespace ppt_cli