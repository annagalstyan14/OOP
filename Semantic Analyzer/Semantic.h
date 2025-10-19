#pragma once
#include <memory>
#include <vector>
#include "../Parser/Parser.h"

namespace ppt_cli {

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer(const std::vector<std::unique_ptr<ICommand>>& commands);
    void analyze();

private:
    void analyzeCommand(const ICommand& command);
    void checkSlideExists(const std::string& number);
    void semanticError(const std::string& message);

    const std::vector<std::unique_ptr<ICommand>>& commands_;
    size_t slide_count_;
};

}