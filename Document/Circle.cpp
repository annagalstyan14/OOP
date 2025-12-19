#include "Circle.h"
#include <algorithm>

namespace ppt {

Circle::Circle(int id, const std::string& name, const std::string& color)
    : BaseSlideObject(id, name, ObjectType::CIRCLE, color) {}

Circle::Circle(const Circle& other) : BaseSlideObject(other) {}

std::unique_ptr<SlideObject> Circle::clone() const {
    return std::make_unique<Circle>(*this);
}

double Circle::getRadius() const {
    return std::min(geometry_.getWidth(), geometry_.getHeight()) / 2.0;
}

Point Circle::getCenter() const {
    return Point(
        geometry_.topLeft.x + geometry_.getWidth() / 2.0,
        geometry_.topLeft.y + geometry_.getHeight() / 2.0
    );
}

} // namespace ppt
