#ifndef SAVE_COMMAND_H_
#define SAVE_COMMAND_H_

#include "ICommand.h"
#include "Argument.h"
#include <vector>

namespace ppt_cli {

class SaveCommand : public ICommand {
public:
    explicit SaveCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}
    void execute(Presentation& pres, Editor& editor) override;
    
private:
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif // SAVE_COMMAND_H_
