#include "AddCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Action/SlideActions.h"
#include "../../Action/ShapeActions.h"
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
            throw std::runtime_error("Usage: add shape <name> <type> [fillColor] [strokeColor] [x] [y] [width] [height]");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add shape to.");

        std::string name;
        ppt::ObjectType type = ppt::ObjectType::RECTANGLE;
        std::string fillColor = "blue";
        std::string strokeColor = "black";
        double x = 0.0, y = 0.0, width = 100.0, height = 100.0;
        
        size_t argIndex = 0;
        
        // Parse name and type: first arg can be name or type
        if (args_.size() > argIndex && args_[argIndex]->isString()) {
            std::string firstArg = args_[argIndex]->asString();
            ppt::ObjectType detectedType = ppt::SlideObject::stringToType(firstArg);
            
            // Check if first arg is a valid shape type
            bool isType = (firstArg == "rectangle" || firstArg == "rect" || 
                          firstArg == "circle" || firstArg == "ellipse" || 
                          firstArg == "line");
            
            if (isType) {
                // First arg is type, auto-generate name
                type = detectedType;
                name = firstArg + "1";
                argIndex++;
            } else {
                // First arg is name
                name = firstArg;
                argIndex++;
                
                // Second arg should be type
                if (args_.size() > argIndex && args_[argIndex]->isString()) {
                    type = ppt::SlideObject::stringToType(args_[argIndex]->asString());
                    argIndex++;
                }
            }
        }
        
        // Parse colors and geometry
        if (args_.size() > argIndex && args_[argIndex]->isString()) {
            fillColor = args_[argIndex]->asString();
            argIndex++;
        }
        if (args_.size() > argIndex && args_[argIndex]->isString()) {
            strokeColor = args_[argIndex]->asString();
            argIndex++;
        }
        if (args_.size() > argIndex && args_[argIndex]->isNumber()) {
            x = args_[argIndex]->asNumber();
            argIndex++;
        }
        if (args_.size() > argIndex && args_[argIndex]->isNumber()) {
            y = args_[argIndex]->asNumber();
            argIndex++;
        }
        if (args_.size() > argIndex && args_[argIndex]->isNumber()) {
            width = args_[argIndex]->asNumber();
            argIndex++;
        }
        if (args_.size() > argIndex && args_[argIndex]->isNumber()) {
            height = args_[argIndex]->asNumber();
        }

        // Create shape with action
        auto action = std::make_unique<ppt::AddShapeAction>(name, type, fillColor);
        editor.executeAction(std::move(action));
        
        // Set position, size, and stroke color on the newly created shape
        ppt::Slide* slide = pres.getSlideAt(pres.size() - 1);
        ppt::SlideObject* obj = slide->getObjectAt(slide->objectCount() - 1);
        if (obj) {
            ppt::Geometry geom(x, y, x + width, y + height);
            obj->setGeometry(geom);
            obj->setStrokeColor(strokeColor);
            if (fillColor == "none") {
                obj->setFillColor("none");
            }
        }
    }
    else {
        throw std::runtime_error("Unknown target: " + target_);
    }
}

} // namespace ppt_cli
