// Command/AddCommand.h
#ifndef ADD_COMMAND_H_
#define ADD_COMMAND_H_

#include "ICommand.h"
#include "Argument.h"
#include "../Document/Presentation.h"
#include <vector>
#include <string>

namespace ppt_cli {

class AddCommand : public ICommand {
public:
    AddCommand(const std::string& target, std::vector<ArgPtr>&& args);
    void execute(Presentation& pres) override;

private:
    std::string target_;
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif // ADD_COMMAND_H_