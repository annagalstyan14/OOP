#ifndef COMMAND_CREATORS_H_
#define COMMAND_CREATORS_H_

#include "CommandCreator.h"
#include "AddCommand.h"
#include "RemoveCommand.h"
#include "ListCommand.h"
#include "SaveCommand.h"
#include "LoadCommand.h"
#include "SetCommand.h"

namespace ppt_cli {

class AddCommandCreator : public CommandCreator {
public:
    std::unique_ptr<ICommand> create(const std::string& target, 
                                      std::vector<ArgPtr>&& args) override {
        return std::make_unique<AddCommand>(target, std::move(args));
    }
};

class RemoveCommandCreator : public CommandCreator {
public:
    std::unique_ptr<ICommand> create(const std::string& target, 
                                      std::vector<ArgPtr>&& args) override {
        return std::make_unique<RemoveCommand>(target, std::move(args));
    }
};

class ListCommandCreator : public CommandCreator {
public:
    std::unique_ptr<ICommand> create(const std::string& target, 
                                      std::vector<ArgPtr>&& args) override {
        return std::make_unique<ListCommand>(target, std::move(args));
    }
};

class SaveCommandCreator : public CommandCreator {
public:
    std::unique_ptr<ICommand> create(const std::string& /*target*/, 
                                      std::vector<ArgPtr>&& args) override {
        return std::make_unique<SaveCommand>(std::move(args));
    }
};

class LoadCommandCreator : public CommandCreator {
public:
    std::unique_ptr<ICommand> create(const std::string& /*target*/, 
                                      std::vector<ArgPtr>&& args) override {
        return std::make_unique<LoadCommand>(std::move(args));
    }
};

class SetCommandCreator : public CommandCreator {
public:
    std::unique_ptr<ICommand> create(const std::string& target, 
                                      std::vector<ArgPtr>&& args) override {
        return std::make_unique<SetCommand>(target, std::move(args));
    }
};

} // namespace ppt_cli

#endif // COMMAND_CREATORS_H_
