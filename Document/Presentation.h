#ifndef PPT_PRESENTATION_H_
#define PPT_PRESENTATION_H_

#include "Slide.h"
#include <vector>
#include <string>
#include <algorithm>

namespace ppt {

class Presentation {
private:
    std::vector<Slide> slides_;
    int nextSlideId_ = 1;

public:
    Presentation() = default;
    
    // Slide operations
    Slide* addSlide(const std::string& title = "Untitled") {
        slides_.emplace_back(nextSlideId_++, title);
        return &slides_.back();
    }
    
    Slide* insertSlide(size_t index, const std::string& title = "Untitled") {
        if (index > slides_.size()) {
            index = slides_.size();
        }
        auto it = slides_.emplace(slides_.begin() + index, nextSlideId_++, title);
        return &(*it);
    }
    
    bool removeSlide(int id) {
        auto it = std::find_if(slides_.begin(), slides_.end(),
                               [id](const Slide& s) { return s.getId() == id; });
        if (it == slides_.end()) return false;
        slides_.erase(it);
        return true;
    }
    
    bool removeSlideAt(size_t index) {
        if (index >= slides_.size()) return false;
        slides_.erase(slides_.begin() + index);
        return true;
    }
    
    // Slide access
    Slide* getSlide(int id) {
        auto it = std::find_if(slides_.begin(), slides_.end(),
                               [id](const Slide& s) { return s.getId() == id; });
        return it != slides_.end() ? &(*it) : nullptr;
    }
    
    const Slide* getSlide(int id) const {
        auto it = std::find_if(slides_.begin(), slides_.end(),
                               [id](const Slide& s) { return s.getId() == id; });
        return it != slides_.end() ? &(*it) : nullptr;
    }
    
    Slide* getSlideAt(size_t index) {
        return index < slides_.size() ? &slides_[index] : nullptr;
    }
    
    const Slide* getSlideAt(size_t index) const {
        return index < slides_.size() ? &slides_[index] : nullptr;
    }
    
    // Getters
    std::vector<Slide>& getSlides() { return slides_; }
    const std::vector<Slide>& getSlides() const { return slides_; }
    size_t size() const { return slides_.size(); }
    bool empty() const { return slides_.empty(); }
    int getNextSlideId() const { return nextSlideId_; }
    
    // Setters
    void setNextSlideId(int id) { nextSlideId_ = id; }
    
    // Clear
    void clear() {
        slides_.clear();
        nextSlideId_ = 1;
    }
    
    // Display
    void display(std::ostream& os = std::cout) const {
        os << "Presentation (" << slides_.size() << " slides):\n";
        if (slides_.empty()) {
            os << "  (empty)\n";
        } else {
            for (const auto& slide : slides_) {
                slide.display(os);
            }
        }
    }
};

} // namespace ppt

#endif // PPT_PRESENTATION_H_
