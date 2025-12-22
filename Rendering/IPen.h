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

class SolidPen : public IPen {
private:
    std::string color_ = "black";
    double width_ = 1.0;
    std::string style_ = "solid";

public:
    SolidPen() = default;
    SolidPen(const std::string& color, double width = 1.0, 
             const std::string& style = "solid")
        : color_(color), width_(width), style_(style) {}
    
    std::string getColor() const override { return color_; }
    void setColor(const std::string& color) override { color_ = color; }
    
    double getWidth() const override { return width_; }
    void setWidth(double width) override { width_ = width; }
    
    std::string getStyle() const override { return style_; }
    void setStyle(const std::string& style) override { style_ = style; }
};

} // namespace ppt

#endif // IPEN_H_
