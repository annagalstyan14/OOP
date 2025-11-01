#ifndef SHAPE_H_
#define SHAPE_H_

#include "BoundingBox.h"
#include "../SlideObject.h"
#include <string>

namespace ppt_cli {

class Shape {
private:
    std::string name;
    ObjectType type;
    std::string color;
    BoundingBox box;

public:
    Shape(std::string n, ObjectType t, std::string c = "black");
    const std::string& getName() const;
    ObjectType getType() const;
    const std::string& getColor() const;
    const BoundingBox& getBoundingBox() const;
    BoundingBox& getBoundingBox();
    void setColor(const std::string& c);
    void move(double dx, double dy);
    void resize(double dw, double dh);
    SlideObject toSlideObject() const;
    void display(std::ostream& os = std::cout) const;
};

} // namespace ppt_cli

#endif // SHAPE_H_