#include "RemoveCommand.h"
#include "../Document/Slide.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

RemoveCommand::RemoveCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}

void RemoveCommand::execute(Presentation& pres) {
    if (args_.empty()) {
        throw std::runtime_error("Usage: remove <slide|text|shape> [id|area|index]");
    }

    const std::string& target = args_[0]->asString();

    if (target == "slide") {
        if (args_.size() < 2 || !args_[1]->isNumber())
            throw std::runtime_error("Usage: remove slide <id> (number)");
        int id = static_cast<int>(args_[1]->asNumber());
        pres.removeSlide(id);
        std::cout << "Removed slide #" << id << "\n";
    }
    else if (target == "text") {
        if (args_.size() < 2)
            throw std::runtime_error("Usage: remove text <area>");
        const std::string& area = args_[1]->asString();
        Slide* current = pres.getSlideAt(pres.size() - 1);
        if (!current)
            throw std::runtime_error("No current slide to remove text from.");
        current->removeText(area);
        std::cout << "Removed text area '" << area << "'\n";
    }
    else if (target == "shape") {
        if (args_.size() < 2 || !args_[1]->isNumber())
            throw std::runtime_error("Usage: remove shape <index>");
        int index = static_cast<int>(args_[1]->asNumber());
        Slide* current = pres.getSlideAt(pres.size() - 1);
        if (!current)
            throw std::runtime_error("No current slide to remove shape from.");

        // Note: Slide::objects is private → add helper or make public
        std::cout << "Shape removal not implemented (index " << index << ")\n";
        // TODO: Add removeObject(int index) to Slide
    }
    else {
        throw std::runtime_error("Unknown remove target: " + target);
    }
}

} // namespace ppt_cli