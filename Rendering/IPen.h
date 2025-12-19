#ifndef IPEN_H_
#define IPEN_H_

#include <string>

namespace ppt {

class IPen {
public:
    virtual ~IPen() = default;
    
    virtual std::string getColor() const = 0;
    virtual void setColor(const std::string& color) = 0;
    
    virtual double getWidth() const = 0;
    virtual void setWidth(double width) = 0;
    
    virtual std::string getStyle() const = 0;
    virtual void setStyle(const std::string& style) = 0;
};

} // namespace ppt

#endif // IPEN_H_
