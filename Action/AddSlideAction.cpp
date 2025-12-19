#include "AddSlideAction.h"
#include <iostream>

namespace ppt_cli {

AddSlideAction::AddSlideAction(const std::string& title)
    : title_(title) {}

void AddSlideAction::execute(Presentation& presentation) {
    Slide* slide = presentation.addSlide(title_);
    addedSlideId_ = slide->getId();
    std::cout << "Added slide #" << addedSlideId_ << " (" << title_ << ")\n";
}

void AddSlideAction::undo(Presentation& presentation) {
    if (addedSlideId_ >= 0) {
        presentation.removeSlide(addedSlideId_);
        std::cout << "Undone: removed slide #" << addedSlideId_ << "\n";
    }
}

std::string AddSlideAction::getDescription() const {
    return "Add slide: " + title_;
}

} // namespace ppt_cli
