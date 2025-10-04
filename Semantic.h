#ifndef SEMANTIC_ANALYZER_H_
#define SEMANTIC_ANALYZER_H_

#include <string>
#include <vector>
#include "Parser.h"

struct SemanticResult {
    bool ok;
    std::string message;
};

class SemanticAnalyzer {
public:
    SemanticAnalyzer() {}
    SemanticResult analyze(const CommandNode& node);
    static bool validatePositions(const std::string& s);
    static std::vector<int> parsePosition(const std::string& s);
private:
    static bool isNumber(const std::string& t);
};

#endif // !SEMANTIC_ANALYZER_H_