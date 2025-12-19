#include "Presentation.h"
#include <algorithm>
#include <iostream>

namespace ppt_cli {

Slide* Presentation::addSlide(const std::string& title) {
    slides_.emplace_back(nextId_++, title);
    return &slides_.back();
}

bool Presentation::removeSlide(int id) {
    auto it = std::find_if(slides_.begin(), slides_.end(),
                           [id](const Slide& s) { return s.getId() == id; });
    if (it == slides_.end()) return false;
    slides_.erase(it);
    return true;
}

Slide* Presentation::getSlide(int id) {
    auto it = std::find_if(slides_.begin(), slides_.end(),
                           [id](const Slide& s) { return s.getId() == id; });
    return it != slides_.end() ? &(*it) : nullptr;
}

const Slide* Presentation::getSlide(int id) const {
    auto it = std::find_if(slides_.begin(), slides_.end(),
                           [id](const Slide& s) { return s.getId() == id; });
    return it != slides_.end() ? &(*it) : nullptr;
}

Slide* Presentation::getSlideAt(size_t index) {
    return index < slides_.size() ? &slides_[index] : nullptr;
}

const Slide* Presentation::getSlideAt(size_t index) const {
    return index < slides_.size() ? &slides_[index] : nullptr;
}

void Presentation::display(std::ostream& os) const {
    os << "Presentation (" << slides_.size() << " slides):\n";
    for (const auto& slide : slides_) {
        slide.display(os);
    }
}

} // namespace ppt_cli
