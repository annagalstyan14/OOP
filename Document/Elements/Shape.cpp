#include "Shape.h"

namespace ppt_cli {

Shape::Shape(int id, const std::string& name, ShapeType type, const std::string& color)
    : id_(id), name_(name), type_(type), color_(color) {}

Shape::Shape(const Shape& other)
    : id_(other.id_)
    , name_(other.name_)
    , type_(other.type_)
    , color_(other.color_)
    , boundingBox_(other.boundingBox_) {}

Shape& Shape::operator=(const Shape& other) {
    if (this != &other) {
        id_ = other.id_;
        name_ = other.name_;
        type_ = other.type_;
        color_ = other.color_;
        boundingBox_ = other.boundingBox_;
    }
    return *this;
}

void Shape::move(double dx, double dy) {
    boundingBox_.move(dx, dy);
}

void Shape::resize(double dw, double dh) {
    boundingBox_.resize(dw, dh);
}

IShape* Shape::clone() const {
    return new Shape(*this);
}

void Shape::display(std::ostream& os) const {
    os << "Shape #" << id_ << ": " << name_ 
       << " (Type: " << IShape::shapeTypeToString(type_)
       << ", Color: " << color_ << ") ";
    boundingBox_.display(os);
    os << "\n";
}

} // namespace ppt_cli
