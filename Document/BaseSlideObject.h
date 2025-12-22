#ifndef BASE_SLIDE_OBJECT_H_
#define BASE_SLIDE_OBJECT_H_

#include "SlideObject.h"

namespace ppt {

class BaseSlideObject : public SlideObject {
protected:
    int id_;
    std::string name_;
    ObjectType type_;
    std::string color_;  // Legacy: for backward compatibility
    std::string fillColor_;    // Fill color (use "none" for outline-only)
    std::string strokeColor_;  // Stroke/border color
    Geometry geometry_;

public:
    BaseSlideObject(int id, const std::string& name, ObjectType type, 
                    const std::string& color = "black");
    BaseSlideObject(const BaseSlideObject& other);
    BaseSlideObject& operator=(const BaseSlideObject& other);
    virtual ~BaseSlideObject() = default;
    
    int getId() const override { return id_; }
    void setId(int id) override { id_ = id; }
    
    const std::string& getName() const override { return name_; }
    void setName(const std::string& name) override { name_ = name; }
    
    ObjectType getType() const override { return type_; }
    
    const std::string& getColor() const override { return color_; }
    void setColor(const std::string& color) override { 
        color_ = color;
        fillColor_ = color;  // Update fill color when legacy color is set
    }
    
    const std::string& getFillColor() const override { return fillColor_; }
    void setFillColor(const std::string& color) override { fillColor_ = color; }
    const std::string& getStrokeColor() const override { return strokeColor_; }
    void setStrokeColor(const std::string& color) override { strokeColor_ = color; }
    
    const Geometry& getGeometry() const override { return geometry_; }
    Geometry& getGeometry() override { return geometry_; }
    void setGeometry(const Geometry& geom) override { geometry_ = geom; }
    
    void move(double dx, double dy) override;
    void resize(double dw, double dh) override;
    
    void display(std::ostream& os = std::cout) const override;
};

} // namespace ppt

#endif // BASE_SLIDE_OBJECT_H_
