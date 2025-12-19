#ifndef SHAPE_H_
#define SHAPE_H_

#include "IShape.h"

namespace ppt_cli {

class Shape : public IShape {
protected:
    int id_;
    std::string name_;
    ShapeType type_;
    std::string color_;
    BoundingBox boundingBox_;

public:
    Shape(int id, const std::string& name, ShapeType type, const std::string& color = "black");
    Shape(const Shape& other);
    Shape& operator=(const Shape& other);
    virtual ~Shape() = default;
    
    // IShape interface implementation
    int getId() const override { return id_; }
    const std::string& getName() const override { return name_; }
    ShapeType getType() const override { return type_; }
    const std::string& getColor() const override { return color_; }
    const BoundingBox& getBoundingBox() const override { return boundingBox_; }
    BoundingBox& getBoundingBox() override { return boundingBox_; }
    
    void setId(int id) override { id_ = id; }
    void setName(const std::string& name) override { name_ = name; }
    void setColor(const std::string& color) override { color_ = color; }
    void setBoundingBox(const BoundingBox& box) override { boundingBox_ = box; }
    
    void move(double dx, double dy) override;
    void resize(double dw, double dh) override;
    
    IShape* clone() const override;
    void display(std::ostream& os = std::cout) const override;
};

} // namespace ppt_cli

#endif // SHAPE_H_
