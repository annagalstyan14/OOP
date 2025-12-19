#ifndef COMMAND_REPOSITORY_H_
#define COMMAND_REPOSITORY_H_

#include "../Command/MetaCommand.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace ppt_cli {

class CommandRepository {
public:
    CommandRepository();

    void registerCommand(std::unique_ptr<MetaCommand> metaCommand);
    MetaCommand* find(const std::string& commandName);
    bool exists(const std::string& commandName) const;

private:
    std::unordered_map<std::string, std::unique_ptr<MetaCommand>> commands_;
};

} // namespace ppt_cli

#endif // COMMAND_REPOSITORY_H_
