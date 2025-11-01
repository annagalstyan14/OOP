// Parser/CommandFactory.cpp
#include "CommandFactory.h"
#include "../Command/AddCommand.h"
#include "../Command/RemoveCommand.h"
#include "../Command/ListCommand.h"
#include <stdexcept>

namespace ppt_cli {

std::unique_ptr<ICommand> CommandFactory::createCommand(
    const std::string& action,
    const std::string& target,
    std::vector<ArgPtr>&& args)   // <-- rvalue reference
{
    if (action == "add") {
        return std::make_unique<AddCommand>(std::move(args));
    }
    if (action == "remove") {
        return std::make_unique<RemoveCommand>(std::move(args));
    }
    if (action == "list") {
        return std::make_unique<ListCommand>(std::move(args));
    }

    throw std::runtime_error("Unknown command: " + action + " " + target);
}

} // namespace ppt_cli