#pragma once
#include <string>
#include "Tokenizer.h"
#include <memory>
#include <vector>

namespace ppt_cli {

enum class State {
    START,
    ACTION,
    TARGET,
    ARGUMENTS,
    DONE,
    ERROR
};

enum class CommandType {
    Add,
    Remove,
    Edit,
    Set,
    Unknown
};

enum class ObjectType {
    Slide,
    Title,
    Shape,
    Bullet,
    Unknown
};

struct Key {
    State state;
    std::string token;
};

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual CommandType getCommandType() const = 0;
    virtual ObjectType getObjectType() const = 0;
    virtual const std::vector<std::string>& getArgs() const = 0;
};

class AddSlide : public ICommand {
public:
    AddSlide(std::vector<std::string> args = {}) : args_(std::move(args)) {}
    CommandType getCommandType() const override { return CommandType::Add; }
    ObjectType getObjectType() const override { return ObjectType::Slide; }
    const std::vector<std::string>& getArgs() const override { return args_; }

private:
    std::vector<std::string> args_;
};

class AddShape : public ICommand {
public:
    AddShape(std::string shape_type) : args_{std::move(shape_type)} {}
    CommandType getCommandType() const override { return CommandType::Add; }
    ObjectType getObjectType() const override { return ObjectType::Shape; }
    const std::vector<std::string>& getArgs() const override { return args_; }

private:
    std::vector<std::string> args_;
};

class AddBullet : public ICommand {
public:
    AddBullet(std::string text) : args_{std::move(text)} {}
    CommandType getCommandType() const override { return CommandType::Add; }
    ObjectType getObjectType() const override { return ObjectType::Bullet; }
    const std::vector<std::string>& getArgs() const override { return args_; }

private:
    std::vector<std::string> args_;
};

class AddTitle : public ICommand {
public:
    AddTitle(std::string text) : args_{std::move(text)} {}
    CommandType getCommandType() const override { return CommandType::Add; }
    ObjectType getObjectType() const override { return ObjectType::Title; }
    const std::vector<std::string>& getArgs() const override { return args_; }

private:
    std::vector<std::string> args_;
};

class SetTitle : public ICommand {
public:
    SetTitle(std::string text) : args_{std::move(text)} {}
    CommandType getCommandType() const override { return CommandType::Set; }
    ObjectType getObjectType() const override { return ObjectType::Title; }
    const std::vector<std::string>& getArgs() const override { return args_; }

private:
    std::vector<std::string> args_;
};

class RemoveSlide : public ICommand {
public:
    RemoveSlide(std::vector<std::string> args = {}) : args_(std::move(args)) {}
    CommandType getCommandType() const override { return CommandType::Remove; }
    ObjectType getObjectType() const override { return ObjectType::Slide; }
    const std::vector<std::string>& getArgs() const override { return args_; }

private:
    std::vector<std::string> args_;
};

class EditSlide : public ICommand {
public:
    EditSlide(std::string number) : args_{std::move(number)} {}
    CommandType getCommandType() const override { return CommandType::Edit; }
    ObjectType getObjectType() const override { return ObjectType::Slide; }
    const std::vector<std::string>& getArgs() const override { return args_; }

private:
    std::vector<std::string> args_;
};

class Parser {
public:
    explicit Parser(std::istream& stream);
    std::unique_ptr<ICommand> parse();
    void processToken(Token token, CommandType& action, ObjectType& target, std::vector<std::string>& args);

private:
    Token nextToken();
    void initializeTransitionTable();

    Tokenizer tokenizer_;
    Token current_token_;
    State current_state_ = State::START;
    static constexpr size_t kTableSize = 20;
    State transition_table_[kTableSize][kTableSize];
};

}