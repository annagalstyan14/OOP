#include "RemoveCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Action/SlideActions.h"
#include "../../Action/ShapeActions.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

RemoveCommand::RemoveCommand(const std::string& target, std::vector<ArgPtr>&& args) 
    : target_(target), args_(std::move(args)) {}

void RemoveCommand::execute(Presentation& pres, Editor& editor) {
    if (target_ == "slide") {
        if (args_.empty() || !args_[0]->isNumber())
            throw std::runtime_error("Usage: remove slide <id> (number)");
        
        int id = static_cast<int>(args_[0]->asNumber());
        auto action = std::make_unique<ppt::RemoveSlideAction>(id);
        editor.executeAction(std::move(action));
    }
    else if (target_ == "text") {
        if (args_.empty())
            throw std::runtime_error("Usage: remove text <id>");
        if (pres.size() == 0)
            throw std::runtime_error("No current slide to remove text from.");
        
        if (!args_[0]->isNumber())
            throw std::runtime_error("Usage: remove text <id> (number)");
        
        int textId = static_cast<int>(args_[0]->asNumber());
        auto action = std::make_unique<ppt::RemoveShapeAction>(textId);
        editor.executeAction(std::move(action));
    }
    else if (target_ == "shape") {
        if (args_.empty() || !args_[0]->isNumber())
            throw std::runtime_error("Usage: remove shape <id>");
        
        int shapeId = static_cast<int>(args_[0]->asNumber());
        auto action = std::make_unique<ppt::RemoveShapeAction>(shapeId);
        editor.executeAction(std::move(action));
    }
    else {
        throw std::runtime_error("Unknown remove target: " + target_);
    }
}

} // namespace ppt_cli
