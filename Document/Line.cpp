#include "Line.h"

namespace ppt {

Line::Line(int id, const std::string& name, const std::string& color)
    : BaseSlideObject(id, name, ObjectType::LINE, color) {}

Line::Line(const Line& other) : BaseSlideObject(other) {}

std::unique_ptr<SlideObject> Line::clone() const {
    return std::make_unique<Line>(*this);
}

Point Line::getStart() const { 
    return geometry_.topLeft; 
}

Point Line::getEnd() const { 
    return geometry_.bottomRight; 
}

void Line::setStart(const Point& p) { 
    geometry_.topLeft = p; 
}

void Line::setEnd(const Point& p) { 
    geometry_.bottomRight = p; 
}

} // namespace ppt
