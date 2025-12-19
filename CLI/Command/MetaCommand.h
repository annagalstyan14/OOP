#ifndef META_COMMAND_H_
#define META_COMMAND_H_

#include "CommandCreator.h"
#include <string>
#include <memory>

namespace ppt_cli {

class MetaCommand {
public:
    MetaCommand(std::string name, 
                std::string description,
                bool requiresTarget,
                std::unique_ptr<CommandCreator> creator)
        : name_(std::move(name))
        , description_(std::move(description))
        , requiresTarget_(requiresTarget)
        , creator_(std::move(creator)) 
    {}

    const std::string& getName() const { return name_; }
    const std::string& getDescription() const { return description_; }
    bool requiresTarget() const { return requiresTarget_; }

    std::unique_ptr<ICommand> createCommand(
        const std::string& target, 
        std::vector<ArgPtr>&& args) 
    {
        return creator_->create(target, std::move(args));
    }

private:
    std::string name_;
    std::string description_;
    bool requiresTarget_;
    std::unique_ptr<CommandCreator> creator_;
};

} // namespace ppt_cli

#endif // META_COMMAND_H_
