#include "RenderCommand.h"
#include <iostream>
#include <stdexcept>

namespace ppt {

RenderCommand::RenderCommand(std::shared_ptr<IPainter> painter,
                             const std::string& outputPath,
                             RenderTarget target)
    : painter_(painter)
    , outputPath_(outputPath)
    , target_(target) {}

void RenderCommand::setTargetId(int id) {
    targetId_ = id;
}

void RenderCommand::execute(const Presentation& pres) {
    switch (target_) {
        case RenderTarget::SLIDE: {
            if (targetId_ >= 0) {
                const Slide* slide = pres.getSlide(targetId_);
                if (!slide) {
                    throw std::runtime_error("Slide not found: " + std::to_string(targetId_));
                }
                renderSlide(*slide, outputPath_);
            } else if (!pres.empty()) {
                // Render current (last) slide
                const Slide* slide = pres.getSlideAt(pres.size() - 1);
                renderSlide(*slide, outputPath_);
            }
            break;
        }
        
        case RenderTarget::PRESENTATION:
            renderPresentation(pres);
            break;
            
        case RenderTarget::OBJECT: {
            if (targetId_ < 0) {
                throw std::runtime_error("Object ID not specified");
            }
            // Find object in current slide
            if (pres.empty()) {
                throw std::runtime_error("No slides in presentation");
            }
            const Slide* slide = pres.getSlideAt(pres.size() - 1);
            const SlideObject* obj = slide->getObject(targetId_);
            if (!obj) {
                throw std::runtime_error("Object not found: " + std::to_string(targetId_));
            }
            renderObject(*obj, outputPath_);
            break;
        }
    }
}

void RenderCommand::renderSlide(const Slide& slide, const std::string& filename) {
    painter_->clear();
    painter_->paintSlide(slide);
    
    if (painter_->saveToFile(filename)) {
        std::cout << "Rendered slide #" << slide.getId() << " to " << filename << "\n";
    } else {
        std::cerr << "Failed to save to " << filename << "\n";
    }
}

void RenderCommand::renderPresentation(const Presentation& pres) {
    for (size_t i = 0; i < pres.size(); ++i) {
        const Slide* slide = pres.getSlideAt(i);
        if (!slide) continue;
        
        std::string filename = outputPath_ + "_" + std::to_string(i + 1) + ".svg";
        renderSlide(*slide, filename);
    }
    
    std::cout << "Rendered " << pres.size() << " slides.\n";
}

void RenderCommand::renderObject(const SlideObject& object, const std::string& filename) {
    painter_->clear();
    painter_->paint(object);
    
    if (painter_->saveToFile(filename)) {
        std::cout << "Rendered object #" << object.getId() << " to " << filename << "\n";
    } else {
        std::cerr << "Failed to save to " << filename << "\n";
    }
}

} // namespace ppt
