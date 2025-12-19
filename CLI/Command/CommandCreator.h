#ifndef COMMAND_CREATOR_H_
#define COMMAND_CREATOR_H_

#include "ICommand.h"
#include "Argument.h"
#include <memory>
#include <vector>
#include <string>

namespace ppt_cli {

class CommandCreator {
public:
    virtual ~CommandCreator() = default;
    virtual std::unique_ptr<ICommand> create(
        const std::string& target, 
        std::vector<ArgPtr>&& args) = 0;
};

} // namespace ppt_cli

#endif // COMMAND_CREATOR_H_
