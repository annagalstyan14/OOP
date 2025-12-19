#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "Shape.h"

namespace ppt_cli {

class Ellipse : public Shape {
public:
    Ellipse(int id, const std::string& name, const std::string& color = "black")
        : Shape(id, name, ShapeType::ELLIPSE, color) {}
    
    Ellipse(const Ellipse& other) : Shape(other) {}
    
    IShape* clone() const override {
        return new Ellipse(*this);
    }
};

} // namespace ppt_cli

#endif // ELLIPSE_H_
