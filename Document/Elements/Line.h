#ifndef LINE_H_
#define LINE_H_

#include "Shape.h"

namespace ppt_cli {

class Line : public Shape {
public:
    Line(int id, const std::string& name, const std::string& color = "black")
        : Shape(id, name, ShapeType::LINE, color) {}
    
    Line(const Line& other) : Shape(other) {}
    
    IShape* clone() const override {
        return new Line(*this);
    }
};

} // namespace ppt_cli

#endif // LINE_H_
