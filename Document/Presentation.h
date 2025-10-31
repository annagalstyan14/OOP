// Document/Presentation.h
#ifndef PRESENTATION_H_
#define PRESENTATION_H_

#include "Slide.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>

namespace ppt_cli {

class Presentation {
private:
    std::vector<std::unique_ptr<Slide>> slides;
    int nextId = 1;

public:
    Presentation() = default;

    Slide* addSlide(const std::string& type = "blank");
    void removeSlide(int id);
    void removeSlideAt(size_t index);
    Slide* getSlide(int id);
    const Slide* getSlide(int id) const;
    Slide* getSlideAt(size_t index);
    const Slide* getSlideAt(size_t index) const;
    size_t size() const { return slides.size(); }

    void display() const;
    void displaySlide(int id) const;
    void displaySlideAt(size_t index) const;

    int getFirstSlideId() const;
    int getLastSlideId() const;
};

} // namespace ppt_cli

#endif // PRESENTATION_H_