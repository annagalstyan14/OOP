#ifndef ISHAPE_H_
#define ISHAPE_H_

#include "BoundingBox.h"
#include <string>
#include <iostream>

namespace ppt_cli {

enum class ShapeType {
    RECTANGLE,
    ELLIPSE,
    TRIANGLE,
    LINE
};

class IShape {
public:
    virtual ~IShape() = default;
    
    // Getters
    virtual int getId() const = 0;
    virtual const std::string& getName() const = 0;
    virtual ShapeType getType() const = 0;
    virtual const std::string& getColor() const = 0;
    virtual const BoundingBox& getBoundingBox() const = 0;
    virtual BoundingBox& getBoundingBox() = 0;
    
    // Setters
    virtual void setId(int id) = 0;
    virtual void setName(const std::string& name) = 0;
    virtual void setColor(const std::string& color) = 0;
    virtual void setBoundingBox(const BoundingBox& box) = 0;
    
    // Operations
    virtual void move(double dx, double dy) = 0;
    virtual void resize(double dw, double dh) = 0;
    
    // Clone for copying
    virtual IShape* clone() const = 0;
    
    // Display
    virtual void display(std::ostream& os = std::cout) const = 0;
    
    // Helper
    static std::string shapeTypeToString(ShapeType type);
    static ShapeType stringToShapeType(const std::string& str);
};

// Inline helper implementations
inline std::string IShape::shapeTypeToString(ShapeType type) {
    switch (type) {
        case ShapeType::RECTANGLE: return "rectangle";
        case ShapeType::ELLIPSE:   return "ellipse";
        case ShapeType::TRIANGLE:  return "triangle";
        case ShapeType::LINE:      return "line";
        default:                   return "unknown";
    }
}

inline ShapeType IShape::stringToShapeType(const std::string& str) {
    if (str == "rectangle") return ShapeType::RECTANGLE;
    if (str == "ellipse")   return ShapeType::ELLIPSE;
    if (str == "triangle")  return ShapeType::TRIANGLE;
    if (str == "line")      return ShapeType::LINE;
    return ShapeType::RECTANGLE;
}

} // namespace ppt_cli

#endif // ISHAPE_H_
