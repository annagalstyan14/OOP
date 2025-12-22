#include "CommandRepository.h"
#include "../Command/CommandCreators.h"

namespace ppt_cli {

CommandRepository::CommandRepository() {
    registerCommand(std::make_unique<MetaCommand>(
        "add", "Add a slide, text, or shape", true,
        std::make_unique<AddCommandCreator>()
    ));

    registerCommand(std::make_unique<MetaCommand>(
        "remove", "Remove a slide, text, or shape", true,
        std::make_unique<RemoveCommandCreator>()
    ));

    registerCommand(std::make_unique<MetaCommand>(
        "list", "List slides, objects, or presentation contents", false,
        std::make_unique<ListCommandCreator>()
    ));

    registerCommand(std::make_unique<MetaCommand>(
        "save", "Save the presentation to a file", false,
        std::make_unique<SaveCommandCreator>()
    ));

    registerCommand(std::make_unique<MetaCommand>(
        "load", "Load a presentation from a file", false,
        std::make_unique<LoadCommandCreator>()
    ));

    registerCommand(std::make_unique<MetaCommand>(
        "set", "Set properties of shapes (fill, stroke, position, size)", true,
        std::make_unique<SetCommandCreator>()
    ));
}

void CommandRepository::registerCommand(std::unique_ptr<MetaCommand> metaCommand) {
    std::string name = metaCommand->getName();
    commands_[name] = std::move(metaCommand);
}

MetaCommand* CommandRepository::find(const std::string& commandName) {
    auto it = commands_.find(commandName);
    return it != commands_.end() ? it->second.get() : nullptr;
}

bool CommandRepository::exists(const std::string& commandName) const {
    return commands_.find(commandName) != commands_.end();
}

} // namespace ppt_cli
