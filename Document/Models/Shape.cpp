#include "Shape.h"
#include <iostream>

namespace ppt_cli {

Shape::Shape(std::string n, ObjectType t, std::string c)
    : name(std::move(n)), type(t), color(std::move(c)) {}

const std::string& Shape::getName() const { return name; }
ObjectType Shape::getType() const { return type; }
const std::string& Shape::getColor() const { return color; }
const BoundingBox& Shape::getBoundingBox() const { return box; }
BoundingBox& Shape::getBoundingBox() { return box; }

void Shape::setColor(const std::string& c) { color = c; }
void Shape::move(double dx, double dy) { box.move(dx, dy); }
void Shape::resize(double dw, double dh) { box.resize(dw, dh); }

SlideObject Shape::toSlideObject() const {
    return SlideObject(name, type, color);
}

void Shape::display(std::ostream& os) const {
    os << name << " (Type:" << static_cast<int>(type)
       << ", Color:" << color << ") ";
    box.display(os);
    os << "\n";
}

} // namespace ppt_cli