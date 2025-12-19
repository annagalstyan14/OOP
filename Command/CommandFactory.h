#ifndef COMMAND_FACTORY_H_
#define COMMAND_FACTORY_H_

#include "ICommand.h"
#include "Argument.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

namespace ppt {

class CommandFactory {
public:
    using CommandCreator = std::function<std::unique_ptr<ICommand>(
        const std::string& target, std::vector<ArgPtr>&& args)>;
    
    CommandFactory();
    
    void registerCommand(const std::string& name, CommandCreator creator);
    
    std::unique_ptr<ICommand> create(const std::string& action,
                                      const std::string& target,
                                      std::vector<ArgPtr>&& args);
    
    bool hasCommand(const std::string& name) const;

private:
    std::unordered_map<std::string, CommandCreator> creators_;
};

} // namespace ppt

#endif // COMMAND_FACTORY_H_
