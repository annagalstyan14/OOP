#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "BaseSlideObject.h"

namespace ppt {

class Rectangle : public BaseSlideObject {
public:
    Rectangle(int id, const std::string& name, const std::string& color = "black");
    Rectangle(const Rectangle& other);
    
    std::unique_ptr<SlideObject> clone() const override;
};

} // namespace ppt

#endif // RECTANGLE_H_
