#include "SVGPainter.h"
#include <fstream>

namespace ppt {

SVGPainter::SVGPainter(int width, int height)
    : canvas_(std::make_unique<SVGCanvas>(width, height)) {}

void SVGPainter::paint(const SlideObject& object) {
    PaintProperties props(object.getColor(), "black", 1.0);
    const Geometry& geom = object.getGeometry();
    
    switch (object.getType()) {
        case ObjectType::RECTANGLE:
            canvas_->drawRectangle(geom, props);
            break;
            
        case ObjectType::CIRCLE: {
            const Circle* circle = dynamic_cast<const Circle*>(&object);
            if (circle) {
                Point center = circle->getCenter();
                double rx = geom.getWidth() / 2.0;
                double ry = geom.getHeight() / 2.0;
                canvas_->drawEllipse(center, rx, ry, props);
            }
            break;
        }
        
        case ObjectType::LINE:
            canvas_->drawLine(geom.topLeft, geom.bottomRight, props);
            break;
            
        case ObjectType::TEXT: {
            const TextObject* text = dynamic_cast<const TextObject*>(&object);
            if (text) {
                canvas_->drawText(
                    geom.topLeft,
                    text->getContent(),
                    text->getFontFamily(),
                    text->getFontSize(),
                    text->getColor()
                );
            }
            break;
        }
    }
    
    // Draw label for shapes (not text)
    if (object.getType() != ObjectType::TEXT) {
        Point labelPos(
            geom.getX() + geom.getWidth() / 2,
            geom.getY() + geom.getHeight() / 2 + 5
        );
        canvas_->drawText(labelPos, object.getName(), "Arial", 12, "white");
    }
}

void SVGPainter::paintSlide(const Slide& slide) {
    // Draw slide title
    Point titlePos(canvas_->getWidth() / 2, 40);
    canvas_->drawText(titlePos, slide.getTitle(), "Arial", 24, "black");
    
    // Draw all objects
    for (const auto& obj : slide.getObjects()) {
        paint(*obj);
    }
}

bool SVGPainter::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) return false;
    file << getOutput();
    return true;
}

std::string SVGPainter::getOutput() const {
    return canvas_->getOutput();
}

void SVGPainter::clear() {
    canvas_->clear();
}

} // namespace ppt
