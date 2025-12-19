#include "RemoveShapeAction.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

RemoveShapeAction::RemoveShapeAction(int shapeId)
    : shapeId_(shapeId) {}

void RemoveShapeAction::execute(Presentation& presentation) {
    if (presentation.size() == 0) {
        throw std::runtime_error("No slide to remove shape from.");
    }
    
    slideIndex_ = presentation.size() - 1;
    Slide* slide = presentation.getSlideAt(slideIndex_);
    
    // Save the shape for undo
    const IShape* shape = slide->getShape(shapeId_);
    if (shape) {
        removedShape_.reset(shape->clone());
    }
    
    if (!slide->removeShape(shapeId_)) {
        throw std::runtime_error("Shape not found: " + std::to_string(shapeId_));
    }
    
    std::cout << "Removed shape #" << shapeId_ << "\n";
}

void RemoveShapeAction::undo(Presentation& presentation) {
    if (removedShape_ && slideIndex_ < presentation.size()) {
        Slide* slide = presentation.getSlideAt(slideIndex_);
        if (slide) {
            IShape* shape = slide->addShape(removedShape_->getName(), 
                                            removedShape_->getType(), 
                                            removedShape_->getColor());
            shape->setBoundingBox(removedShape_->getBoundingBox());
            std::cout << "Undone: restored shape '" << removedShape_->getName() << "'\n";
        }
    }
}

std::string RemoveShapeAction::getDescription() const {
    return "Remove shape #" + std::to_string(shapeId_);
}

} // namespace ppt_cli
