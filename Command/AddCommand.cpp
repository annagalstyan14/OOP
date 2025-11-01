#include "AddCommand.h"
#include "../Document/Slide.h"
#include "../Document/Models/Shape.h"
#include "../Document/SlideObject.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

AddCommand::AddCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}

void AddCommand::execute(Presentation& pres) {
    if (args_.size() < 2) {
        throw std::runtime_error("Usage: add <slide|text|shape> <type> [args...]");
    }

    const std::string& target = args_[0]->asString();  // slide, text, shape
    const std::string& type   = args_[1]->asString();  // title, circle, etc.

    if (target == "slide") {
        Slide* s = pres.addSlide(type);
        std::cout << "Added slide #" << s->getId() << " (" << type << ")\n";
    }
    else if (target == "text") {
        if (args_.size() < 4)
            throw std::runtime_error("Usage: add text <area> \"content\"");

        const std::string& area = args_[2]->asString();
        const std::string& content = args_[3]->asString();

        Slide* current = pres.getSlideAt(pres.size() - 1);
        if (!current)
            throw std::runtime_error("No slide to add text to.");

        current->addText(area, content);
        std::cout << "Added text to area '" << area << "'\n";
    }
    else if (target == "shape") {
        if (args_.size() < 3)
            throw std::runtime_error("Usage: add shape <name>");

        const std::string& name = args_[2]->asString();

        Slide* current = pres.getSlideAt(pres.size() - 1);
        if (!current)
            throw std::runtime_error("No slide to add shape to.");

        Shape shape(name, ObjectType::SHAPE, "black");
        current->addObject(shape.toSlideObject());
        std::cout << "Added shape '" << name << "'\n";
    }
    else {
        throw std::runtime_error("Unknown add target: " + target);
    }
}

} // namespace ppt_cli