#include "Presentation.h"
#include <algorithm>
#include <stdexcept>

namespace ppt_cli {

Slide* Presentation::addSlide(const std::string& type) {
    auto slide = std::make_unique<Slide>(nextId++, type);
    slides.push_back(std::move(slide));
    return slides.back().get();
}

void Presentation::removeSlide(int id) {
    auto it = std::remove_if(slides.begin(), slides.end(),
        [id](const auto& s) { return s->getId() == id; });
    if (it == slides.end())
        throw std::runtime_error("Slide with id " + std::to_string(id) + " not found.");
    slides.erase(it, slides.end());
}

void Presentation::removeSlideAt(size_t index) {
    if (index >= slides.size())
        throw std::runtime_error("Slide index out of range.");
    slides.erase(slides.begin() + index);
}

Slide* Presentation::getSlide(int id) {
    auto it = std::find_if(slides.begin(), slides.end(),
        [id](const auto& s) { return s->getId() == id; });
    return (it != slides.end()) ? it->get() : nullptr;
}

const Slide* Presentation::getSlide(int id) const {
    auto it = std::find_if(slides.begin(), slides.end(),
        [id](const auto& s) { return s->getId() == id; });
    return (it != slides.end()) ? it->get() : nullptr;
}

Slide* Presentation::getSlideAt(size_t index) {
    return (index < slides.size()) ? slides[index].get() : nullptr;
}

const Slide* Presentation::getSlideAt(size_t index) const {
    return (index < slides.size()) ? slides[index].get() : nullptr;
}

void Presentation::display() const {
    std::cout << "=== Presentation (" << slides.size() << " slides) ===\n";
    for (const auto& slide : slides) {
        slide->display();
        std::cout << "\n";
    }
}

void Presentation::displaySlide(int id) const {
    const Slide* s = getSlide(id);
    if (!s) {
        std::cout << "Slide #" << id << " not found.\n";
        return;
    }
    s->display();
}

void Presentation::displaySlideAt(size_t index) const {
    const Slide* s = getSlideAt(index);
    if (!s) {
        std::cout << "Slide at index " << index << " not found.\n";
        return;
    }
    s->display();
}

int Presentation::getFirstSlideId() const {
    return slides.empty() ? -1 : slides.front()->getId();
}

int Presentation::getLastSlideId() const {
    return slides.empty() ? -1 : slides.back()->getId();
}

} // namespace ppt_cli