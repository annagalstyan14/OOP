#include "RemoveSlideAction.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

RemoveSlideAction::RemoveSlideAction(int slideId)
    : slideId_(slideId) {}

void RemoveSlideAction::execute(Presentation& presentation) {
    // Find and save the slide before removing
    const auto& slides = presentation.getSlides();
    for (size_t i = 0; i < slides.size(); ++i) {
        if (slides[i].getId() == slideId_) {
            removedSlide_ = slides[i];
            removedIndex_ = i;
            break;
        }
    }
    
    if (!presentation.removeSlide(slideId_)) {
        throw std::runtime_error("Slide not found: " + std::to_string(slideId_));
    }
    
    std::cout << "Removed slide #" << slideId_ << "\n";
}

void RemoveSlideAction::undo(Presentation& presentation) {
    if (removedSlide_.has_value()) {
        auto& slides = presentation.getSlides();
        if (removedIndex_ <= slides.size()) {
            slides.insert(slides.begin() + removedIndex_, removedSlide_.value());
            std::cout << "Undone: restored slide #" << slideId_ << "\n";
        }
    }
}

std::string RemoveSlideAction::getDescription() const {
    return "Remove slide #" + std::to_string(slideId_);
}

} // namespace ppt_cli
