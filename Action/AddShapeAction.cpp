#include "AddShapeAction.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

AddShapeAction::AddShapeAction(const std::string& name, ShapeType type, const std::string& color)
    : name_(name), type_(type), color_(color) {}

void AddShapeAction::execute(Presentation& presentation) {
    if (presentation.size() == 0) {
        throw std::runtime_error("No slide to add shape to.");
    }
    
    slideIndex_ = presentation.size() - 1;
    Slide* slide = presentation.getSlideAt(slideIndex_);
    IShape* shape = slide->addShape(name_, type_, color_);
    addedShapeId_ = shape->getId();
    
    std::cout << "Added shape '" << name_ << "' (id: " << addedShapeId_ << ")\n";
}

void AddShapeAction::undo(Presentation& presentation) {
    if (addedShapeId_ >= 0 && slideIndex_ < presentation.size()) {
        Slide* slide = presentation.getSlideAt(slideIndex_);
        if (slide) {
            slide->removeShape(addedShapeId_);
            std::cout << "Undone: removed shape #" << addedShapeId_ << "\n";
        }
    }
}

std::string AddShapeAction::getDescription() const {
    return "Add shape: " + name_;
}

} // namespace ppt_cli
