#include "CommandFactory.h"
#include "../Action/SlideActions.h"
#include "../Action/ShapeActions.h"
#include <iostream>
#include <stdexcept>

namespace ppt {

// Anonymous namespace for command implementations
namespace {

class AddCommand : public ICommand {
private:
    std::string target_;
    std::vector<ArgPtr> args_;

public:
    AddCommand(const std::string& target, std::vector<ArgPtr>&& args)
        : target_(target), args_(std::move(args)) {}
    
    void execute(Presentation& pres, Editor& editor) override {
        if (target_ == "slide") {
            std::string title = "Untitled";
            if (!args_.empty() && args_[0]->isString()) {
                title = args_[0]->asString();
            }
            editor.executeAction(std::make_unique<AddSlideAction>(title));
        }
        else if (target_ == "shape" || target_ == "rectangle" || 
                 target_ == "circle" || target_ == "line") {
            if (args_.empty()) {
                throw std::runtime_error("Usage: add shape <name> [type] [color]");
            }
            
            std::string name = args_[0]->asString();
            ObjectType type = ObjectType::RECTANGLE;
            std::string color = "blue";
            
            if (target_ != "shape") {
                type = SlideObject::stringToType(target_);
            } else if (args_.size() > 1 && args_[1]->isString()) {
                type = SlideObject::stringToType(args_[1]->asString());
            }
            
            if (args_.size() > 2 && args_[2]->isString()) {
                color = args_[2]->asString();
            } else if (target_ != "shape" && args_.size() > 1 && args_[1]->isString()) {
                color = args_[1]->asString();
            }
            
            editor.executeAction(std::make_unique<AddShapeAction>(name, type, color));
        }
        else if (target_ == "text") {
            if (args_.size() < 2) {
                throw std::runtime_error("Usage: add text <name> <content>");
            }
            std::string name = args_[0]->asString();
            std::string content = args_[1]->asString();
            std::string color = args_.size() > 2 ? args_[2]->asString() : "black";
            
            editor.executeAction(std::make_unique<AddTextAction>(name, content, color));
        }
        else {
            throw std::runtime_error("Unknown add target: " + target_);
        }
    }
};

class RemoveCommand : public ICommand {
private:
    std::string target_;
    std::vector<ArgPtr> args_;

public:
    RemoveCommand(const std::string& target, std::vector<ArgPtr>&& args)
        : target_(target), args_(std::move(args)) {}
    
    void execute(Presentation& pres, Editor& editor) override {
        if (target_ == "slide") {
            if (args_.empty() || !args_[0]->isNumber()) {
                throw std::runtime_error("Usage: remove slide <id>");
            }
            int id = args_[0]->asInt();
            editor.executeAction(std::make_unique<RemoveSlideAction>(id));
        }
        else if (target_ == "shape" || target_ == "text") {
            if (args_.empty() || !args_[0]->isNumber()) {
                throw std::runtime_error("Usage: remove " + target_ + " <id>");
            }
            int id = args_[0]->asInt();
            editor.executeAction(std::make_unique<RemoveShapeAction>(id));
        }
        else {
            throw std::runtime_error("Unknown remove target: " + target_);
        }
    }
};

class ListCommand : public ICommand {
private:
    std::string target_;
    std::vector<ArgPtr> args_;

public:
    ListCommand(const std::string& target, std::vector<ArgPtr>&& args)
        : target_(target), args_(std::move(args)) {}
    
    void execute(Presentation& pres, Editor& /*editor*/) override {
        if (target_.empty() || target_ == "all") {
            pres.display();
        }
        else if (target_ == "slides") {
            std::cout << "Slides (" << pres.size() << "):\n";
            for (size_t i = 0; i < pres.size(); ++i) {
                const Slide* s = pres.getSlideAt(i);
                std::cout << "  [" << i << "] Slide #" << s->getId() 
                          << " (" << s->getTitle() << ") - " 
                          << s->objectCount() << " objects\n";
            }
        }
        else if (target_ == "current" || target_ == "shapes") {
            if (pres.empty()) {
                std::cout << "No slides.\n";
                return;
            }
            const Slide* current = pres.getSlideAt(pres.size() - 1);
            if (target_ == "current") {
                current->display();
            } else {
                std::cout << "Objects on current slide (" << current->objectCount() << "):\n";
                for (const auto& obj : current->getObjects()) {
                    std::cout << "  ";
                    obj->display();
                }
            }
        }
        else {
            std::cout << "Unknown list target: " << target_ << "\n";
        }
    }
};

} // anonymous namespace

CommandFactory::CommandFactory() {
    registerCommand("add", [](const std::string& target, std::vector<ArgPtr>&& args) {
        return std::make_unique<AddCommand>(target, std::move(args));
    });
    
    registerCommand("remove", [](const std::string& target, std::vector<ArgPtr>&& args) {
        return std::make_unique<RemoveCommand>(target, std::move(args));
    });
    
    registerCommand("list", [](const std::string& target, std::vector<ArgPtr>&& args) {
        return std::make_unique<ListCommand>(target, std::move(args));
    });
}

void CommandFactory::registerCommand(const std::string& name, CommandCreator creator) {
    creators_[name] = std::move(creator);
}

std::unique_ptr<ICommand> CommandFactory::create(const std::string& action,
                                                  const std::string& target,
                                                  std::vector<ArgPtr>&& args) {
    auto it = creators_.find(action);
    if (it == creators_.end()) {
        throw std::runtime_error("Unknown command: " + action);
    }
    return it->second(target, std::move(args));
}

bool CommandFactory::hasCommand(const std::string& name) const {
    return creators_.find(name) != creators_.end();
}

} // namespace ppt
