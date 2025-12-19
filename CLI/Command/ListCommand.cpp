#include "ListCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include <iostream>

namespace ppt_cli {

ListCommand::ListCommand(const std::string& target, std::vector<ArgPtr>&& args) 
    : target_(target), args_(std::move(args)) {}

void ListCommand::execute(Presentation& pres, Editor& /*editor*/) {
    if (target_.empty() || target_ == "all") {
        pres.display();
        return;
    }

    if (target_ == "slides") {
        std::cout << "Slides (" << pres.size() << "):\n";
        for (size_t i = 0; i < pres.size(); ++i) {
            const Slide* s = pres.getSlideAt(i);
            std::cout << "  [" << i << "] Slide #" << s->getId() 
                      << " (" << s->getTitle() << ") - " 
                      << s->shapeCount() << " shapes\n";
        }
    }
    else if (target_ == "current") {
        if (pres.size() == 0) {
            std::cout << "No slides.\n";
            return;
        }
        Slide* current = pres.getSlideAt(pres.size() - 1);
        std::cout << "Current slide:\n";
        current->display();
    }
    else if (target_ == "shapes") {
        if (pres.size() == 0) {
            std::cout << "No slides.\n";
            return;
        }
        Slide* current = pres.getSlideAt(pres.size() - 1);
        std::cout << "Shapes on current slide (" << current->shapeCount() << "):\n";
        for (const auto& shape : current->getShapes()) {
            std::cout << "  ";
            shape->display();
        }
    }
    else {
        std::cout << "Unknown list target: " << target_ << "\n";
    }
}

} // namespace ppt_cli
