#ifndef ISHAPE_H_
#define ISHAPE_H_

#include "BoundingBox.h"
#include <string>
#include <memory>
#include <algorithm>

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
    
    virtual int getId() const = 0;
    virtual void setId(int id) = 0;
    
    virtual const std::string& getName() const = 0;
    virtual void setName(const std::string& name) = 0;
    
    virtual ShapeType getType() const = 0;
    
    virtual const std::string& getColor() const = 0;
    virtual void setColor(const std::string& color) = 0;
    
    virtual const BoundingBox& getBoundingBox() const = 0;
    virtual void setBoundingBox(const BoundingBox& box) = 0;
    
    virtual std::unique_ptr<IShape> clone() const = 0;
    
    static std::string shapeTypeToString(ShapeType type) {
        switch (type) {
            case ShapeType::RECTANGLE: return "rectangle";
            case ShapeType::ELLIPSE:   return "ellipse";
            case ShapeType::TRIANGLE:  return "triangle";
            case ShapeType::LINE:      return "line";
            default:                   return "unknown";
        }
    }
    
    static ShapeType stringToShapeType(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        
        if (lower == "rectangle" || lower == "rect") return ShapeType::RECTANGLE;
        if (lower == "ellipse" || lower == "circle") return ShapeType::ELLIPSE;
        if (lower == "triangle") return ShapeType::TRIANGLE;
        if (lower == "line") return ShapeType::LINE;
        return ShapeType::RECTANGLE;
    }
};

// Concrete shape implementation
class Shape : public IShape {
private:
    int id_ = 0;
    std::string name_;
    ShapeType type_;
    std::string color_;
    BoundingBox boundingBox_;

public:
    Shape(int id, const std::string& name, ShapeType type, const std::string& color = "black")
        : id_(id), name_(name), type_(type), color_(color) {}
    
    Shape(const Shape& other) = default;
    
    int getId() const override { return id_; }
    void setId(int id) override { id_ = id; }
    
    const std::string& getName() const override { return name_; }
    void setName(const std::string& name) override { name_ = name; }
    
    ShapeType getType() const override { return type_; }
    
    const std::string& getColor() const override { return color_; }
    void setColor(const std::string& color) override { color_ = color; }
    
    const BoundingBox& getBoundingBox() const override { return boundingBox_; }
    void setBoundingBox(const BoundingBox& box) override { boundingBox_ = box; }
    
    std::unique_ptr<IShape> clone() const override {
        return std::make_unique<Shape>(*this);
    }
};

} // namespace ppt_cli

#endif // ISHAPE_H_
