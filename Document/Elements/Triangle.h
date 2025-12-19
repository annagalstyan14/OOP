#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"

namespace ppt_cli {

class Triangle : public Shape {
public:
    Triangle(int id, const std::string& name, const std::string& color = "black")
        : Shape(id, name, ShapeType::TRIANGLE, color) {}
    
    Triangle(const Triangle& other) : Shape(other) {}
    
    IShape* clone() const override {
        return new Triangle(*this);
    }
};

} // namespace ppt_cli

#endif // TRIANGLE_H_
