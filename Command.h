#pragma once
#include <string>
#include <vector>
#include <map>

namespace ppt_cli {

struct CommandNode {
    std::string cmd;
    std::string object;
    std::map<std::string, std::string> flags;
    std::vector<std::string> args;
    std::vector<std::string> positionArgs;
};

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};

}