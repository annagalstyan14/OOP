#ifndef SET_COMMAND_H_
#define SET_COMMAND_H_

#include "ICommand.h"
#include "Argument.h"
#include <vector>
#include <string>

namespace ppt_cli {

class SetCommand : public ICommand {
public:
    SetCommand(const std::string& target, std::vector<ArgPtr>&& args);
    void execute(Presentation& pres, Editor& editor) override;

private:
    std::string target_;
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif // SET_COMMAND_H_

