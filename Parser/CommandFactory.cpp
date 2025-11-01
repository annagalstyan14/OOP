#include "CommandFactory.h"
#include "../Command/AddCommand.h"
#include "../Command/LoadCommand.h"
#include "../Command/SaveCommand.h"
#include "../Command/RemoveCommand.h"
#include "../Command/ListCommand.h"
#include <stdexcept>

namespace ppt_cli {

std::unique_ptr<ICommand> CommandFactory::createCommand(
    const std::string& action,
    const std::string& target,
    std::vector<ArgPtr>&& args)
{
    if (action == "add") {
        return std::make_unique<AddCommand>(target, std::move(args));
    }
    if (action == "remove") {
        return std::make_unique<RemoveCommand>(target, std::move(args));
    }
    if (action == "list") {
        return std::make_unique<ListCommand>(target, std::move(args));
    }
    if (action == "save") {
        return std::make_unique<SaveCommand>(std::move(args));
    }
    if (action == "load") {
        return std::make_unique<LoadCommand>(std::move(args));
    }

    throw std::runtime_error("Unknown command: " + action);
}

} // namespace ppt_cli