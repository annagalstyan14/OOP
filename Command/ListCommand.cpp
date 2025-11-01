#include "ListCommand.h"
#include "../Document/Slide.h"
#include <iostream>

namespace ppt_cli {

ListCommand::ListCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}

void ListCommand::execute(Presentation& pres) {
    if (args_.empty()) {
        pres.display();
        return;
    }

    const std::string& target = args_[0]->asString();

    if (target == "slides") {
        std::cout << "Slides (" << pres.size() << "):\n";
        for (size_t i = 0; i < pres.size(); ++i) {
            const Slide* s = pres.getSlideAt(i);
            std::cout << "  [" << i << "] Slide #" << s->getId() << " (" << s->getText() << ")\n";
        }
    }
    else if (target == "current") {
        if (pres.size() == 0) {
            std::cout << "No slides.\n";
            return;
        }
        Slide* current = pres.getSlideAt(pres.size() - 1);
        std::cout << "Current slide:\n";
        current->display();
    }
    else {
        std::cout << "Unknown list target: " << target << "\n";
    }
}

} // namespace ppt_cli