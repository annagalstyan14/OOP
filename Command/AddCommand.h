// Command/AddCommand.h
#ifndef ADD_COMMAND_H_
#define ADD_COMMAND_H_

#include "ICommand.h"
#include "Argument.h"
#include "../Document/Presentation.h"
#include <vector>

namespace ppt_cli {

class AddCommand : public Command {
public:
    explicit AddCommand(std::vector<ArgPtr> args);
    void execute(Presentation& pres) override;

private:
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif // ADD_COMMAND_H_