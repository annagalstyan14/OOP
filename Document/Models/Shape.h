#ifndef SHAPE_H_
#define SHAPE_H_

#include "BoundingBox.h"
#include "SlideObject.h"
#include <string>
#include <iostream>

namespace ppt_cli {

class Shape {
private:
    std::string name;
    ObjectType type;
    std::string color;
    BoundingBox box;

public:
    Shape(std::string name, ObjectType type, std::string color = "black")
        : name(std::move(name)), type(type), color(std::move(color)) {}

    const std::string& getName() const { return name; }
    ObjectType getType() const { return type; }
    const std::string& getColor() const { return color; }
    const BoundingBox& getBoundingBox() const { return box; }
    BoundingBox& getBoundingBox() { return box; }

    void setColor(const std::string& c) { color = c; }
    void move(double dx, double dy) { box.move(dx, dy); }
    void resize(double dw, double dh) { box.resize(dw, dh); }

    SlideObject toSlideObject() const {
        return SlideObject(name, type, color);
    }

    void display(std::ostream& os = std::cout) const {
        os << name << " (Type: " << static_cast<int>(type)
           << ", Color: " << color << ") ";
        box.display(os);
        os << "\n";
    }
};

} // namespace ppt_cli

#endif // SHAPE_H_