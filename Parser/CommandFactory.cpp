// Parser/CommandFactory.cpp
#include "CommandFactory.h"
#include "../Command/AddCommand.h"
#include "../Command/RemoveCommand.h"
#include "../Command/ListCommand.h"
#include <stdexcept>

namespace ppt_cli {

// ← REMOVE 'static' HERE
std::unique_ptr<ICommand> CommandFactory::createCommand(
    const std::string& action,
    std::vector<ArgPtr>&& args)
{
    if (action == "add") {
        if (args.empty()) {
            throw std::runtime_error("add requires a target (e.g. slide, text)");
        }
        return std::make_unique<AddCommand>(std::move(args));
    }
    if (action == "remove") {
        return std::make_unique<RemoveCommand>(std::move(args));
    }
    if (action == "list") {
        return std::make_unique<ListCommand>(std::move(args));
    }

    throw std::runtime_error("Unknown command: " + action);
}

} // namespace ppt_cli