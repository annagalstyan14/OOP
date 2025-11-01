#ifndef COMMAND_FACTORY_H_
#define COMMAND_FACTORY_H_

#include "../Command/ICommand.h"
#include "../Command/Argument.h"
#include <string>
#include <vector>
#include <memory>

namespace ppt_cli {

using ArgPtr = std::unique_ptr<Argument>;

class CommandFactory {
public:
static std::unique_ptr<ICommand> createCommand(
    const std::string& action,
    const std::string& target,
    std::vector<ArgPtr>&& args);
};

} // namespace ppt_cli

#endif // COMMAND_FACTORY_H_