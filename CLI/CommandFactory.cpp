#include "CommandFactory.h"
#include "../Document/JSONSerializer.h"
#include "../Action/SlideActions.h"
#include "../Action/ShapeActions.h"
#include "../Rendering/SVGPainter.h"
#include "../Rendering/RenderCommand.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

std::unique_ptr<ICommand> CommandFactory::create(const ParsedCommand& parsed) {
    if (!parsed.isValid()) {
        return nullptr;
    }
    
    // Make a mutable copy of args
    std::vector<ArgPtr> args;
    for (const auto& arg : parsed.args) {
        if (arg->isString()) {
            args.push_back(std::make_unique<Argument>(arg->asString()));
        } else {
            args.push_back(std::make_unique<Argument>(arg->asNumber()));
        }
    }
    
    const std::string& action = parsed.action;
    
    if (action == "add") {
        return createAddCommand(parsed.target, args);
    } else if (action == "remove") {
        return createRemoveCommand(parsed.target, args);
    } else if (action == "list") {
        return createListCommand(parsed.target, args);
    } else if (action == "save") {
        return createSaveCommand(args);
    } else if (action == "load") {
        return createLoadCommand(args);
    } else if (action == "render" || action == "export") {
        return createRenderCommand(args);
    }
    
    return nullptr;
}

std::unique_ptr<ICommand> CommandFactory::createAddCommand(
    const std::string& target, std::vector<ArgPtr>& args) {
    return std::make_unique<AddCommand>(target, std::move(args));
}

std::unique_ptr<ICommand> CommandFactory::createRemoveCommand(
    const std::string& target, std::vector<ArgPtr>& args) {
    return std::make_unique<RemoveCommand>(target, std::move(args));
}

std::unique_ptr<ICommand> CommandFactory::createListCommand(
    const std::string& target, std::vector<ArgPtr>& args) {
    return std::make_unique<ListCommand>(target, std::move(args));
}

std::unique_ptr<ICommand> CommandFactory::createSaveCommand(std::vector<ArgPtr>& args) {
    return std::make_unique<SaveCommand>(std::move(args));
}

std::unique_ptr<ICommand> CommandFactory::createLoadCommand(std::vector<ArgPtr>& args) {
    return std::make_unique<LoadCommand>(std::move(args));
}

std::unique_ptr<ICommand> CommandFactory::createRenderCommand(std::vector<ArgPtr>& args) {
    return std::make_unique<RenderCommand>(std::move(args));
}

// ============================================================================
// AddCommand
// ============================================================================
AddCommand::AddCommand(const std::string& target, std::vector<ArgPtr> args)
    : target_(target), args_(std::move(args)) {}

void AddCommand::execute(Presentation& pres, Editor& editor) {
    if (target_ == "slide") {
        std::string title = "Untitled";
        if (!args_.empty() && args_[0]->isString()) {
            title = args_[0]->asString();
        }
        auto action = std::make_unique<ppt::AddSlideAction>(title);
        editor.executeAction(std::move(action));
    }
    else if (target_ == "text") {
        if (args_.size() < 2)
            throw std::runtime_error("Usage: add text <area> \"content\"");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add text to.");

        const std::string& area = args_[0]->asString();
        const std::string& content = args_[1]->asString();

        ppt::Slide* current = pres.getSlideAt(pres.size() - 1);
        current->addText(area, content);
        std::cout << "Added text to area '" << area << "'\n";
    }
    else if (target_ == "shape") {
        if (args_.empty())
            throw std::runtime_error("Usage: add shape <name> [type] [color]");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add shape to.");

        const std::string& name = args_[0]->asString();
        
        ppt::ObjectType type = ppt::ObjectType::RECTANGLE;
        std::string color = "blue";
        
        if (args_.size() > 1 && args_[1]->isString()) {
            type = ppt::SlideObject::stringToType(args_[1]->asString());
        }
        if (args_.size() > 2 && args_[2]->isString()) {
            color = args_[2]->asString();
        }

        auto action = std::make_unique<ppt::AddShapeAction>(name, type, color);
        editor.executeAction(std::move(action));
    }
    else {
        throw std::runtime_error("Unknown target: " + target_);
    }
}

// ============================================================================
// RemoveCommand
// ============================================================================
RemoveCommand::RemoveCommand(const std::string& target, std::vector<ArgPtr> args)
    : target_(target), args_(std::move(args)) {}

void RemoveCommand::execute(Presentation& pres, Editor& editor) {
    if (target_ == "slide") {
        if (args_.empty() || !args_[0]->isNumber())
            throw std::runtime_error("Usage: remove slide <id>");
        
        int id = static_cast<int>(args_[0]->asNumber());
        auto action = std::make_unique<ppt::RemoveSlideAction>(id);
        editor.executeAction(std::move(action));
    }
    else if (target_ == "text" || target_ == "shape") {
        if (args_.empty() || !args_[0]->isNumber())
            throw std::runtime_error("Usage: remove " + target_ + " <id>");
        if (pres.size() == 0)
            throw std::runtime_error("No current slide.");
        
        int id = static_cast<int>(args_[0]->asNumber());
        auto action = std::make_unique<ppt::RemoveShapeAction>(id);
        editor.executeAction(std::move(action));
    }
    else {
        throw std::runtime_error("Unknown remove target: " + target_);
    }
}

// ============================================================================
// ListCommand
// ============================================================================
ListCommand::ListCommand(const std::string& target, std::vector<ArgPtr> args)
    : target_(target), args_(std::move(args)) {}

void ListCommand::execute(Presentation& pres, Editor& /*editor*/) {
    if (target_.empty() || target_ == "all") {
        pres.display();
        return;
    }

    if (target_ == "slides") {
        std::cout << "Slides (" << pres.size() << "):\n";
        for (size_t i = 0; i < pres.size(); ++i) {
            const ppt::Slide* s = pres.getSlideAt(i);
            std::cout << "  [" << i << "] Slide #" << s->getId() 
                      << " (" << s->getTitle() << ") - " 
                      << s->objectCount() << " objects\n";
        }
    }
    else if (target_ == "current") {
        if (pres.size() == 0) {
            std::cout << "No slides.\n";
            return;
        }
        ppt::Slide* current = pres.getSlideAt(pres.size() - 1);
        std::cout << "Current slide:\n";
        current->display();
    }
    else if (target_ == "shapes" || target_ == "text") {
        if (pres.size() == 0) {
            std::cout << "No slides.\n";
            return;
        }
        ppt::Slide* current = pres.getSlideAt(pres.size() - 1);
        std::cout << "Objects on current slide (" << current->objectCount() << "):\n";
        for (const auto& obj : current->getObjects()) {
            std::cout << "  ";
            obj->display();
        }
    }
    else {
        std::cout << "Unknown list target: " << target_ << "\n";
    }
}

// ============================================================================
// SaveCommand
// ============================================================================
SaveCommand::SaveCommand(std::vector<ArgPtr> args) : args_(std::move(args)) {}

void SaveCommand::execute(Presentation& pres, Editor& /*editor*/) {
    std::string filename = "presentation.json";
    
    if (!args_.empty() && args_[0]->isString()) {
        filename = args_[0]->asString();
    }
    
    if (ppt::Serializer::saveToFile(pres, filename)) {
        std::cout << "Saved to " << filename << "\n";
    } else {
        std::cout << "Failed to save\n";
    }
}

// ============================================================================
// LoadCommand
// ============================================================================
LoadCommand::LoadCommand(std::vector<ArgPtr> args) : args_(std::move(args)) {}

void LoadCommand::execute(Presentation& pres, Editor& editor) {
    std::string filename = "presentation.json";
    
    if (!args_.empty() && args_[0]->isString()) {
        filename = args_[0]->asString();
    }
    
    if (ppt::Deserializer::loadFromFile(pres, filename)) {
        editor.clearHistory();
        std::cout << "Loaded from " << filename << "\n";
    } else {
        std::cout << "Failed to load (file not found or invalid)\n";
    }
}

// ============================================================================
// RenderCommand
// ============================================================================
RenderCommand::RenderCommand(std::vector<ArgPtr> args) : args_(std::move(args)) {}

void RenderCommand::execute(Presentation& pres, Editor& /*editor*/) {
    if (pres.size() == 0) {
        std::cout << "No slides to render.\n";
        return;
    }
    
    std::string baseName = "slide";
    if (!args_.empty() && args_[0]->isString()) {
        baseName = args_[0]->asString();
    }
    
    auto painter = std::make_shared<ppt::SVGPainter>(800, 600);
    ppt::RenderCommand renderCmd(painter, baseName, 
                                  ppt::RenderCommand::RenderTarget::PRESENTATION);
    renderCmd.execute(pres);
}

} // namespace ppt_cli
