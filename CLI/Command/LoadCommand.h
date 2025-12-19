#ifndef LOAD_COMMAND_H_
#define LOAD_COMMAND_H_

#include "ICommand.h"
#include "Argument.h"
#include <vector>

namespace ppt_cli {

class LoadCommand : public ICommand {
public:
    explicit LoadCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}
    void execute(Presentation& pres, Editor& editor) override;
    
private:
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif // LOAD_COMMAND_H_
