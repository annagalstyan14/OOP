#ifndef COMMAND_FACTORY_H_
#define COMMAND_FACTORY_H_

#include "ICommand.h"
#include "Parser.h"
#include <memory>
#include <string>

namespace ppt_cli {

class CommandFactory {
public:
    static std::unique_ptr<ICommand> create(const ParsedCommand& parsed);
    
    static std::unique_ptr<ICommand> createAddCommand(
        const std::string& target, std::vector<ArgPtr>& args);
    
    static std::unique_ptr<ICommand> createRemoveCommand(
        const std::string& target, std::vector<ArgPtr>& args);
    
    static std::unique_ptr<ICommand> createListCommand(
        const std::string& target, std::vector<ArgPtr>& args);
    
    static std::unique_ptr<ICommand> createSaveCommand(std::vector<ArgPtr>& args);
    
    static std::unique_ptr<ICommand> createLoadCommand(std::vector<ArgPtr>& args);
    
    static std::unique_ptr<ICommand> createRenderCommand(std::vector<ArgPtr>& args);
};

// ============================================================================
// Concrete Command Classes
// ============================================================================

class AddCommand : public ICommand {
public:
    AddCommand(const std::string& target, std::vector<ArgPtr> args);
    void execute(Presentation& pres, Editor& editor) override;
private:
    std::string target_;
    std::vector<ArgPtr> args_;
};

class RemoveCommand : public ICommand {
public:
    RemoveCommand(const std::string& target, std::vector<ArgPtr> args);
    void execute(Presentation& pres, Editor& editor) override;
private:
    std::string target_;
    std::vector<ArgPtr> args_;
};

class ListCommand : public ICommand {
public:
    ListCommand(const std::string& target, std::vector<ArgPtr> args);
    void execute(Presentation& pres, Editor& editor) override;
private:
    std::string target_;
    std::vector<ArgPtr> args_;
};

class SaveCommand : public ICommand {
public:
    explicit SaveCommand(std::vector<ArgPtr> args);
    void execute(Presentation& pres, Editor& editor) override;
private:
    std::vector<ArgPtr> args_;
};

class LoadCommand : public ICommand {
public:
    explicit LoadCommand(std::vector<ArgPtr> args);
    void execute(Presentation& pres, Editor& editor) override;
private:
    std::vector<ArgPtr> args_;
};

class RenderCommand : public ICommand {
public:
    explicit RenderCommand(std::vector<ArgPtr> args);
    void execute(Presentation& pres, Editor& editor) override;
private:
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli

#endif // COMMAND_FACTORY_H_
