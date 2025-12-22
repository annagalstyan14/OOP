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
    
    virtual std::string getPattern() const = 0;
    virtual void setPattern(const std::string& pattern) = 0;
};

class SolidBrush : public IBrush {
private:
    std::string color_ = "white";
    double opacity_ = 1.0;
    std::string pattern_ = "solid";

public:
    SolidBrush() = default;
    explicit SolidBrush(const std::string& color, double opacity = 1.0)
        : color_(color), opacity_(opacity) {}
    
    std::string getColor() const override { return color_; }
    void setColor(const std::string& color) override { color_ = color; }
    
    double getOpacity() const override { return opacity_; }
    void setOpacity(double opacity) override { opacity_ = opacity; }
    
    std::string getPattern() const override { return pattern_; }
    void setPattern(const std::string& pattern) override { pattern_ = pattern; }
};

} // namespace ppt

#endif // IBRUSH_H_
