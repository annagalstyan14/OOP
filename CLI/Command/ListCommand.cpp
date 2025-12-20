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
            const ppt::Slide* s = pres.getSlideAt(i);
            std::cout << "  [" << i << "] Slide #" << s->getId() 
                      << " (" << s->getTitle() << ") - " 
                      << s->objectCount() << " objects\n";
        }
    }
    else if (target_ == "current") {
        if (pres.size() == 0) {
            std::cout << "No slides.\n";
            return;
        }
        ppt::Slide* current = pres.getSlideAt(pres.size() - 1);
        std::cout << "Current slide:\n";
        current->display();
    }
    else if (target_ == "shapes") {
        if (pres.size() == 0) {
            std::cout << "No slides.\n";
            return;
        }
        ppt::Slide* current = pres.getSlideAt(pres.size() - 1);
        std::cout << "Objects on current slide (" << current->objectCount() << "):\n";
        for (const auto& obj : current->getObjects()) {
            std::cout << "  ";
            obj->display();
        }
    }
    else {
        std::cout << "Unknown list target: " << target_ << "\n";
    }
}

} // namespace ppt_cli
