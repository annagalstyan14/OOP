#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "BaseSlideObject.h"

namespace ppt {

class Circle : public BaseSlideObject {
public:
    Circle(int id, const std::string& name, const std::string& color = "black");
    Circle(const Circle& other);
    
    std::unique_ptr<SlideObject> clone() const override;
    
    double getRadius() const;
    Point getCenter() const;
};

} // namespace ppt

#endif // CIRCLE_H_
