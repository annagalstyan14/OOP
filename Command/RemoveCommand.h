#ifndef REMOVE_COMMAND_H_
#define REMOVE_COMMAND_H_

#include "ICommand.h"
#include "Argument.h"
#include "../Document/Presentation.h"
#include <vector>
#include <memory>
#include <string>

namespace ppt_cli {

class RemoveCommand : public ICommand {
public:
    RemoveCommand(const std::string& target, std::vector<ArgPtr>&& args);
    void execute(Presentation& pres) override;

private:
    std::string target_;
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif // REMOVE_COMMAND_H_