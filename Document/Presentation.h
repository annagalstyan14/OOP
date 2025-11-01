#pragma once
#include "Slide.h"
#include <vector>
#include <string>

namespace ppt_cli {

class Presentation {
public:
    Slide* addSlide(const std::string& title);
    bool removeSlide(int id);
    Slide* getSlide(int id);
    Slide* getSlideAt(size_t index);
    const Slide* getSlideAt(size_t index) const;
    const std::vector<Slide>& getSlides() const { return slides_; }
    size_t size() const { return slides_.size(); }
    void display() const;

    // JSON SAVE/LOAD
    bool saveToFile(const std::string& filename = "presentation.json") const;
    bool loadFromFile(const std::string& filename = "presentation.json");

private:
    std::vector<Slide> slides_;
    int nextId_ = 1;
};

} // namespace ppt_cli