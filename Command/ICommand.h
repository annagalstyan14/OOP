#pragma once
#include <memory>
#include <vector>
#include <string>

namespace ppt_cli {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};

} // namespace ppt_cli