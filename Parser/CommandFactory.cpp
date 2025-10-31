#include "CommandFactory.h"
#include <iostream>

namespace ppt_cli {

class StubCommand : public ICommand {
public:
    void execute() override {}
};

std::unique_ptr<ICommand> CommandFactory::createCommand(
    const std::string& action, const std::string& target,
    const std::vector<std::string>& args) {

    if (action.empty()) throw std::runtime_error("No command");
    if (target.empty()) throw std::runtime_error("No object");

    if (action == "add") {
        if (target == "slide") {
            if (!args.empty()) throw std::runtime_error("add slide takes no arguments");
            std::cout << "[PARSED] add slide\n";
            return std::make_unique<StubCommand>();
        }
        if (target == "title" || target == "bullet" || target == "shape") {
            if (args.size() != 1) throw std::runtime_error(target + " requires one argument");
            std::cout << "[PARSED] add " << target << " [" << args[0] << "]\n";
            return std::make_unique<StubCommand>();
        }
    }

    if (action == "remove" && target == "slide" && args.empty()) {
        std::cout << "[PARSED] remove slide\n";
        return std::make_unique<StubCommand>();
    }

    throw std::runtime_error("Invalid command: " + action + " " + target);
}

} // namespace ppt_cli