#pragma once
#include "../Command/ICommand.h"
#include <vector>
#include <memory>

namespace ppt_cli {

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer(const std::vector<std::unique_ptr<ICommand>>& commands);
    void analyze();

private:
    void semanticError(const std::string& msg);

    const std::vector<std::unique_ptr<ICommand>>& commands_;
    //size_t slide_count_ = 0;
};

} // namespace ppt_cli