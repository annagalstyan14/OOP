#ifndef PPT_CLI_PRESENTATION_H_
#define PPT_CLI_PRESENTATION_H_

#include "Slide_cli.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

namespace ppt_cli {

class Presentation {
private:
    std::vector<Slide> slides_;
    int nextId_ = 1;

public:
    Presentation() = default;
    
    // Slide operations
    Slide* addSlide(const std::string& title = "Untitled") {
        slides_.emplace_back(nextId_++, title);
        return &slides_.back();
    }
    
    bool removeSlide(int id) {
        auto it = std::find_if(slides_.begin(), slides_.end(),
                               [id](const Slide& s) { return s.getId() == id; });
        if (it == slides_.end()) return false;
        slides_.erase(it);
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
    int getNextId() const { return nextId_; }
    
    // Setters
    void setNextId(int id) { nextId_ = id; }
    
    // Clear
    void clear() {
        slides_.clear();
        nextId_ = 1;
    }
    
    // Display
    void display(std::ostream& os = std::cout) const {
        os << "Presentation (" << slides_.size() << " slides):\n";
        for (const auto& slide : slides_) {
            slide.display(os);
        }
    }
};

} // namespace ppt_cli

// Type alias for backward compatibility with code using `using Presentation = ppt::Presentation`
// This allows the CLI code to use either namespace

#endif // PPT_CLI_PRESENTATION_H_
