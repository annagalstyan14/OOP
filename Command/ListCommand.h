#ifndef LIST_COMMAND_H_
#define LIST_COMMAND_H_

#include "ICommand.h"
#include "Argument.h"
#include "../Document/Presentation.h"
#include <vector>
#include <memory>

namespace ppt_cli {

class ListCommand : public ICommand {
public:
    explicit ListCommand(std::vector<ArgPtr>&& args);
    void execute(Presentation& pres) override;

private:
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif