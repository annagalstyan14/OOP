#include "Rectangle.h"

namespace ppt {

Rectangle::Rectangle(int id, const std::string& name, const std::string& color)
    : BaseSlideObject(id, name, ObjectType::RECTANGLE, color) {}

Rectangle::Rectangle(const Rectangle& other) : BaseSlideObject(other) {}

std::unique_ptr<SlideObject> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}

} // namespace ppt
