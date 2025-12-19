#include "Presentation.h"
#include <algorithm>

namespace ppt {

Slide* Presentation::addSlide(const std::string& title) {
    slides_.emplace_back(nextSlideId_++, title);
    return &slides_.back();
}

Slide* Presentation::insertSlide(size_t index, const std::string& title) {
    if (index > slides_.size()) {
        index = slides_.size();
    }
    auto it = slides_.emplace(slides_.begin() + index, nextSlideId_++, title);
    return &(*it);
}

bool Presentation::removeSlide(int id) {
    auto it = std::find_if(slides_.begin(), slides_.end(),
                           [id](const Slide& s) { return s.getId() == id; });
    if (it == slides_.end()) return false;
    slides_.erase(it);
    return true;
}

bool Presentation::removeSlideAt(size_t index) {
    if (index >= slides_.size()) return false;
    slides_.erase(slides_.begin() + index);
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

void Presentation::clear() {
    slides_.clear();
    nextSlideId_ = 1;
}

void Presentation::display(std::ostream& os) const {
    os << "Presentation (" << slides_.size() << " slides):\n";
    if (slides_.empty()) {
        os << "  (empty)\n";
    } else {
        for (const auto& slide : slides_) {
            slide.display(os);
        }
    }
}

} // namespace ppt
