#include "AddCommand.h"
#include "../Document/Slide.h"
#include "../Document/Models/Shape.h"
#include "../Document/SlideObject.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

AddCommand::AddCommand(const std::string& target, std::vector<ArgPtr>&& args) 
    : target_(target), args_(std::move(args)) {}

void AddCommand::execute(Presentation& pres) {
    if (target_ == "slide") {
        std::string layout = "blank";
        if (!args_.empty()) {
            if (!args_[0]->isString())
                throw std::runtime_error("Slide layout must be a string");
            layout = args_[0]->asString();
        }
        Slide* s = pres.addSlide(layout);
        std::cout << "Added slide #" << s->getId() << " (" << layout << ")\n";
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
            throw std::runtime_error("Usage: add shape <name>");
        if (pres.size() == 0)
            throw std::runtime_error("No slide to add shape to.");

        const std::string& name = args_[0]->asString();
        Shape shape(name, ObjectType::SHAPE, "black");
        Slide* current = pres.getSlideAt(pres.size() - 1);
        current->addObject(shape.toSlideObject());
        std::cout << "Added shape '" << name << "'\n";
    }
    else {
        throw std::runtime_error("Unknown target: " + target_);
    }
}

} // namespace ppt_cli