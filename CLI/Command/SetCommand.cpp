#include "SetCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

SetCommand::SetCommand(const std::string& target, std::vector<ArgPtr>&& args) 
    : target_(target), args_(std::move(args)) {}

void SetCommand::execute(Presentation& pres, Editor& editor) {
    if (target_ == "shape") {
        if (args_.size() < 3)
            throw std::runtime_error("Usage: set shape <id> <property> <value> [value2]");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to modify shape in.");
        
        if (!args_[0]->isNumber())
            throw std::runtime_error("Shape ID must be a number");
        
        int shapeId = static_cast<int>(args_[0]->asNumber());
        std::string property = args_[1]->asString();
        
        // Find the shape
        ppt::SlideObject* obj = nullptr;
        for (size_t i = 0; i < pres.size(); ++i) {
            ppt::Slide* slide = pres.getSlideAt(i);
            if (slide) {
                obj = slide->getObject(shapeId);
                if (obj) break;
            }
        }
        
        if (!obj) {
            throw std::runtime_error("Shape not found: " + std::to_string(shapeId));
        }
        
        // Handle different properties
        if (property == "fill" || property == "fillcolor") {
            if (args_.size() < 3 || !args_[2]->isString())
                throw std::runtime_error("Usage: set shape <id> fill <color>");
            obj->setFillColor(args_[2]->asString());
            std::cout << "Set fill color of shape #" << shapeId << " to " << args_[2]->asString() << "\n";
        }
        else if (property == "stroke" || property == "strokecolor" || property == "border") {
            if (args_.size() < 3 || !args_[2]->isString())
                throw std::runtime_error("Usage: set shape <id> stroke <color>");
            obj->setStrokeColor(args_[2]->asString());
            std::cout << "Set stroke color of shape #" << shapeId << " to " << args_[2]->asString() << "\n";
        }
        else if (property == "position" || property == "pos") {
            if (args_.size() < 4 || !args_[2]->isNumber() || !args_[3]->isNumber())
                throw std::runtime_error("Usage: set shape <id> position <x> <y>");
            double x = args_[2]->asNumber();
            double y = args_[3]->asNumber();
            ppt::Geometry& geom = obj->getGeometry();
            double width = geom.getWidth();
            double height = geom.getHeight();
            geom.setPosition(x, y);
            std::cout << "Set position of shape #" << shapeId << " to (" << x << ", " << y << ")\n";
        }
        else if (property == "size") {
            if (args_.size() < 4 || !args_[2]->isNumber() || !args_[3]->isNumber())
                throw std::runtime_error("Usage: set shape <id> size <width> <height>");
            double width = args_[2]->asNumber();
            double height = args_[3]->asNumber();
            ppt::Geometry& geom = obj->getGeometry();
            double x = geom.getX();
            double y = geom.getY();
            geom.setSize(width, height);
            std::cout << "Set size of shape #" << shapeId << " to " << width << "x" << height << "\n";
        }
        else if (property == "geometry" || property == "geom") {
            if (args_.size() < 6 || !args_[2]->isNumber() || !args_[3]->isNumber() || 
                !args_[4]->isNumber() || !args_[5]->isNumber())
                throw std::runtime_error("Usage: set shape <id> geometry <x> <y> <width> <height>");
            double x = args_[2]->asNumber();
            double y = args_[3]->asNumber();
            double width = args_[4]->asNumber();
            double height = args_[5]->asNumber();
            ppt::Geometry geom(x, y, x + width, y + height);
            obj->setGeometry(geom);
            std::cout << "Set geometry of shape #" << shapeId << " to (" << x << ", " << y 
                      << ", " << width << "x" << height << ")\n";
        }
        else {
            throw std::runtime_error("Unknown property: " + property + 
                ". Valid: fill, stroke, position, size, geometry");
        }
    }
    else {
        throw std::runtime_error("Unknown set target: " + target_);
    }
}

} // namespace ppt_cli

