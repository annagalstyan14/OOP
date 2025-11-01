#include "AddCommand.h"
#include "../Document/Slide.h"
#include "../Document/Models/Shape.h"
#include "../Document/SlideObject.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

AddCommand::AddCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}

void AddCommand::execute(Presentation& pres) {
    if (args_.empty()) {
        throw std::runtime_error("Usage: add <slide|text|shape> [args...]");
    }

    const std::string& target = args_[0]->asString();

    if (target == "slide") {
        std::string layout = "blank";
        if (args_.size() > 1) {
            if (!args_[1]->isString())
                throw std::runtime_error("Slide layout must be a string");
            layout = args_[1]->asString();
        }
        Slide* s = pres.addSlide(layout);
        std::cout << "Added slide #" << s->getId() << " (" << layout << ")\n";
    }
    else if (target == "text") {
        if (args_.size() < 3)
            throw std::runtime_error("Usage: add text <area> \"content\"");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add text to.");

        const std::string& area = args_[1]->asString();
        const std::string& content = args_[2]->asString();

        Slide* current = pres.getSlideAt(pres.size() - 1);
        current->addText(area, content);
        std::cout << "Added text to area '" << area << "'\n";
    }
    else if (target == "shape") {
        if (args_.size() < 2)
            throw std::runtime_error("Usage: add shape <name>");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add shape to.");

        const std::string& name = args_[1]->asString();
        Shape shape(name, ObjectType::SHAPE, "black");
        Slide* current = pres.getSlideAt(pres.size() - 1);
        current->addObject(shape.toSlideObject());
        std::cout << "Added shape '" << name << "'\n";
    }
    else {
        throw std::runtime_error("Unknown target: " + target);
    }
}

} // namespace ppt_cli