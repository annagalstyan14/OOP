#ifndef PRESENTATION_H_
#define PRESENTATION_H_

#include "Slide.h"
#include <vector>
#include <string>

namespace ppt_cli {

class Presentation {
public:
    Presentation() = default;
    
    // Slide operations
    Slide* addSlide(const std::string& title = "Untitled");
    bool removeSlide(int id);
    Slide* getSlide(int id);
    const Slide* getSlide(int id) const;
    Slide* getSlideAt(size_t index);
    const Slide* getSlideAt(size_t index) const;
    
    // Getters
    std::vector<Slide>& getSlides() { return slides_; }
    const std::vector<Slide>& getSlides() const { return slides_; }
    size_t size() const { return slides_.size(); }
    int getNextId() const { return nextId_; }
    void setNextId(int id) { nextId_ = id; }
    
    // Display
    void display(std::ostream& os = std::cout) const;

private:
    std::vector<Slide> slides_;
    int nextId_ = 1;
};

} // namespace ppt_cli

#endif // PRESENTATION_H_
