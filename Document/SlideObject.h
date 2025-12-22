#ifndef SLIDE_OBJECT_H_
#define SLIDE_OBJECT_H_

#include "../Geometry/Geometry.h"
#include <string>
#include <memory>
#include <iostream>

namespace ppt {

enum class ObjectType {
    RECTANGLE,
    CIRCLE,
    LINE,
    TEXT
};

class SlideObject {
public:
    virtual ~SlideObject() = default;
    
    virtual int getId() const = 0;
    virtual void setId(int id) = 0;
    
    virtual const std::string& getName() const = 0;
    virtual void setName(const std::string& name) = 0;
    
    virtual ObjectType getType() const = 0;
    
    virtual const std::string& getColor() const = 0;
    virtual void setColor(const std::string& color) = 0;
    
    virtual const Geometry& getGeometry() const = 0;
    virtual Geometry& getGeometry() = 0;
    virtual void setGeometry(const Geometry& geom) = 0;
    
    virtual void move(double dx, double dy) = 0;
    virtual void resize(double dw, double dh) = 0;
    
    virtual std::unique_ptr<SlideObject> clone() const = 0;
    
    virtual void display(std::ostream& os = std::cout) const = 0;
    
    static std::string typeToString(ObjectType type) {
        switch (type) {
            case ObjectType::RECTANGLE: return "rectangle";
            case ObjectType::CIRCLE:    return "circle";
            case ObjectType::LINE:      return "line";
            case ObjectType::TEXT:      return "text";
            default:                    return "unknown";
        }
    }
    
    static ObjectType stringToType(const std::string& str) {
        if (str == "rectangle" || str == "rect") return ObjectType::RECTANGLE;
        if (str == "circle" || str == "ellipse") return ObjectType::CIRCLE;
        if (str == "line") return ObjectType::LINE;
        if (str == "text") return ObjectType::TEXT;
        return ObjectType::RECTANGLE;
    }
};

} // namespace ppt

#endif // SLIDE_OBJECT_H_
