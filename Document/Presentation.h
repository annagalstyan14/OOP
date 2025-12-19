#ifndef PRESENTATION_H_
#define PRESENTATION_H_

#include "Slide.h"
#include <vector>
#include <string>

namespace ppt {

class Presentation {
private:
    std::vector<Slide> slides_;
    int nextSlideId_ = 1;

public:
    Presentation() = default;
    
    // Slide operations
    Slide* addSlide(const std::string& title = "Untitled");
    Slide* insertSlide(size_t index, const std::string& title = "Untitled");
    bool removeSlide(int id);
    bool removeSlideAt(size_t index);
    
    // Slide access
    Slide* getSlide(int id);
    const Slide* getSlide(int id) const;
    Slide* getSlideAt(size_t index);
    const Slide* getSlideAt(size_t index) const;
    
    // Getters
    std::vector<Slide>& getSlides() { return slides_; }
    const std::vector<Slide>& getSlides() const { return slides_; }
    size_t size() const { return slides_.size(); }
    bool empty() const { return slides_.empty(); }
    int getNextSlideId() const { return nextSlideId_; }
    
    // Setters
    void setNextSlideId(int id) { nextSlideId_ = id; }
    
    // Clear
    void clear();
    
    // Display
    void display(std::ostream& os = std::cout) const;
};

} // namespace ppt

#endif // PRESENTATION_H_
