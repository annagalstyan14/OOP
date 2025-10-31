#pragma once
#include "../ICommand.h"
#include <memory>
#include <string>
#include <vector>

namespace ppt_cli {

class CommandFactory {
public:
    static std::unique_ptr<ICommand> createCommand(  // ← MATCH NAME
        const std::string& action,
        const std::string& target,
        const std::vector<std::string>& args);
};

} // namespace ppt_cli