#ifndef IBRUSH_H_
#define IBRUSH_H_

#include <string>

namespace ppt {

class IBrush {
public:
    virtual ~IBrush() = default;
    
    virtual std::string getColor() const = 0;
    virtual void setColor(const std::string& color) = 0;
    
    virtual double getOpacity() const = 0;
    virtual void setOpacity(double opacity) = 0;
};

} // namespace ppt

#endif // IBRUSH_H_
