#ifndef ICANVAS_H_
#define ICANVAS_H_

#include "../Geometry/Geometry.h"
#include "PaintProperties.h"
#include <string>

namespace ppt {

class ICanvas {
public:
    virtual ~ICanvas() = default;
    
    virtual void drawRectangle(const Geometry& geom, const PaintProperties& props) = 0;
    virtual void drawCircle(const Point& center, double radius, const PaintProperties& props) = 0;
    virtual void drawEllipse(const Point& center, double rx, double ry, const PaintProperties& props) = 0;
    virtual void drawLine(const Point& start, const Point& end, const PaintProperties& props) = 0;
    virtual void drawText(const Point& position, const std::string& text, 
                          const std::string& font, int fontSize, const std::string& color) = 0;
    
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    
    virtual std::string getOutput() const = 0;
    virtual void clear() = 0;
};

} // namespace ppt

#endif // ICANVAS_H_
