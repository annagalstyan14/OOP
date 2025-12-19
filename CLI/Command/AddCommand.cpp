#include "AddCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Action/AddSlideAction.h"
#include "../../Action/AddShapeAction.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

AddCommand::AddCommand(const std::string& target, std::vector<ArgPtr>&& args) 
    : target_(target), args_(std::move(args)) {}

void AddCommand::execute(Presentation& pres, Editor& editor) {
    if (target_ == "slide") {
        std::string title = "Untitled";
        if (!args_.empty() && args_[0]->isString()) {
            title = args_[0]->asString();
        }
        auto action = std::make_unique<AddSlideAction>(title);
        editor.executeAction(std::move(action));
    }
    else if (target_ == "text") {
        if (args_.size() < 2)
            throw std::runtime_error("Usage: add text <area> \"content\"");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add text to.");

        const std::string& area = args_[0]->asString();
        const std::string& content = args_[1]->asString();

        Slide* current = pres.getSlideAt(pres.size() - 1);
        current->addText(area, content);
        std::cout << "Added text to area '" << area << "'\n";
    }
    else if (target_ == "shape") {
        if (args_.empty())
            throw std::runtime_error("Usage: add shape <name> [type] [color]");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add shape to.");

        const std::string& name = args_[0]->asString();
        
        ShapeType type = ShapeType::RECTANGLE;
        std::string color = "blue";
        
        if (args_.size() > 1 && args_[1]->isString()) {
            type = IShape::stringToShapeType(args_[1]->asString());
        }
        if (args_.size() > 2 && args_[2]->isString()) {
            color = args_[2]->asString();
        }

        auto action = std::make_unique<AddShapeAction>(name, type, color);
        editor.executeAction(std::move(action));
    }
    else {
        throw std::runtime_error("Unknown target: " + target_);
    }
}

} // namespace ppt_cli
