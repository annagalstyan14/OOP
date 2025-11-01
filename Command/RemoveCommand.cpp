#include "RemoveCommand.h"
#include "../Document/Slide.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

RemoveCommand::RemoveCommand(const std::string& target, std::vector<ArgPtr>&& args) 
    : target_(target), args_(std::move(args)) {}

void RemoveCommand::execute(Presentation& pres) {
    if (target_ == "slide") {
        if (args_.empty() || !args_[0]->isNumber())
            throw std::runtime_error("Usage: remove slide <id> (number)");
        int id = static_cast<int>(args_[0]->asNumber());
        if (!pres.removeSlide(id))
            throw std::runtime_error("Slide not found: " + std::to_string(id));
        std::cout << "Removed slide #" << id << "\n";
    }
    else if (target_ == "text") {
        if (args_.empty())
            throw std::runtime_error("Usage: remove text <area>");
        const std::string& area = args_[0]->asString();
        Slide* current = pres.getSlideAt(pres.size() - 1);
        if (!current)
            throw std::runtime_error("No current slide to remove text from.");
        current->removeText(area);
        std::cout << "Removed text area '" << area << "'\n";
    }
    else if (target_ == "shape") {
        if (args_.empty() || !args_[0]->isNumber())
            throw std::runtime_error("Usage: remove shape <index>");
        int index = static_cast<int>(args_[0]->asNumber());
        Slide* current = pres.getSlideAt(pres.size() - 1);
        if (!current)
            throw std::runtime_error("No current slide to remove shape from.");

        // Note: Need to add removeObject method to Slide class
        std::cout << "Shape removal not implemented (index " << index << ")\n";
        // TODO: Add removeObject(int index) to Slide
    }
    else {
        throw std::runtime_error("Unknown remove target: " + target_);
    }
}

} // namespace ppt_cli