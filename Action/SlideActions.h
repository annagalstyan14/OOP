#ifndef SLIDE_ACTIONS_H_
#define SLIDE_ACTIONS_H_

#include "IAction.h"
#include <optional>
#include <iostream>

namespace ppt {

class AddSlideAction : public IAction {
private:
    std::string title_;
    int addedSlideId_ = -1;

public:
    explicit AddSlideAction(const std::string& title = "Untitled")
        : title_(title) {}
    
    void execute(Presentation& pres) override {
        Slide* slide = pres.addSlide(title_);
        addedSlideId_ = slide->getId();
        std::cout << "Added slide #" << addedSlideId_ << " (" << title_ << ")\n";
    }
    
    void undo(Presentation& pres) override {
        if (addedSlideId_ >= 0) {
            pres.removeSlide(addedSlideId_);
            std::cout << "Undone: removed slide #" << addedSlideId_ << "\n";
        }
    }
    
    std::string getDescription() const override {
        return "Add slide: " + title_;
    }
    
    int getAddedSlideId() const { return addedSlideId_; }
};

class RemoveSlideAction : public IAction {
private:
    int slideId_;
    std::optional<Slide> removedSlide_;
    size_t removedIndex_ = 0;

public:
    explicit RemoveSlideAction(int slideId) : slideId_(slideId) {}
    
    void execute(Presentation& pres) override {
        // Save slide before removing
        const auto& slides = pres.getSlides();
        for (size_t i = 0; i < slides.size(); ++i) {
            if (slides[i].getId() == slideId_) {
                removedSlide_ = slides[i];
                removedIndex_ = i;
                break;
            }
        }
        
        if (!pres.removeSlide(slideId_)) {
            throw std::runtime_error("Slide not found: " + std::to_string(slideId_));
        }
        
        std::cout << "Removed slide #" << slideId_ << "\n";
    }
    
    void undo(Presentation& pres) override {
        if (removedSlide_.has_value()) {
            auto& slides = pres.getSlides();
            if (removedIndex_ <= slides.size()) {
                slides.insert(slides.begin() + removedIndex_, removedSlide_.value());
                std::cout << "Undone: restored slide #" << slideId_ << "\n";
            }
        }
    }
    
    std::string getDescription() const override {
        return "Remove slide #" + std::to_string(slideId_);
    }
};

} // namespace ppt

#endif // SLIDE_ACTIONS_H_
