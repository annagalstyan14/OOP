#include "BaseSlideObject.h"

namespace ppt {

BaseSlideObject::BaseSlideObject(int id, const std::string& name, ObjectType type, 
                                  const std::string& color)
    : id_(id), name_(name), type_(type), color_(color) {}

BaseSlideObject::BaseSlideObject(const BaseSlideObject& other)
    : id_(other.id_)
    , name_(other.name_)
    , type_(other.type_)
    , color_(other.color_)
    , geometry_(other.geometry_) {}

BaseSlideObject& BaseSlideObject::operator=(const BaseSlideObject& other) {
    if (this != &other) {
        id_ = other.id_;
        name_ = other.name_;
        type_ = other.type_;
        color_ = other.color_;
        geometry_ = other.geometry_;
    }
    return *this;
}

void BaseSlideObject::move(double dx, double dy) {
    geometry_.move(dx, dy);
}

void BaseSlideObject::resize(double dw, double dh) {
    geometry_.resize(dw, dh);
}

void BaseSlideObject::display(std::ostream& os) const {
    os << typeToString(type_) << " #" << id_ << ": " << name_ 
       << " (Color: " << color_ << ") "
       << "[" << geometry_.topLeft.x << "," << geometry_.topLeft.y << " - "
       << geometry_.bottomRight.x << "," << geometry_.bottomRight.y << "]\n";
}

} // namespace ppt
