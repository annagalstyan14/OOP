#ifndef LINE_H_
#define LINE_H_

#include "BaseSlideObject.h"

namespace ppt {

class Line : public BaseSlideObject {
public:
    Line(int id, const std::string& name, const std::string& color = "black");
    Line(const Line& other);
    
    std::unique_ptr<SlideObject> clone() const override;
    
    Point getStart() const;
    Point getEnd() const;
    void setStart(const Point& p);
    void setEnd(const Point& p);
};

} // namespace ppt

#endif // LINE_H_
