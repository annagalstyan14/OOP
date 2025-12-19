#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"

namespace ppt_cli {

class Rectangle : public Shape {
public:
    Rectangle(int id, const std::string& name, const std::string& color = "black")
        : Shape(id, name, ShapeType::RECTANGLE, color) {}
    
    Rectangle(const Rectangle& other) : Shape(other) {}
    
    IShape* clone() const override {
        return new Rectangle(*this);
    }
};

} // namespace ppt_cli

#endif // RECTANGLE_H_
