#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include <iostream>
#include <algorithm>

namespace ppt_cli {

struct Point {
    double x = 0.0;
    double y = 0.0;
    
    Point() = default;
    Point(double x, double y) : x(x), y(y) {}
};

struct BoundingBox {
    Point topLeft;
    Point bottomRight;

    BoundingBox() : topLeft(0, 0), bottomRight(100, 100) {}
    BoundingBox(double x1, double y1, double x2, double y2);
    BoundingBox(const Point& tl, const Point& br);

    double getX() const { return topLeft.x; }
    double getY() const { return topLeft.y; }
    double getWidth() const { return bottomRight.x - topLeft.x; }
    double getHeight() const { return bottomRight.y - topLeft.y; }
    
    void setPosition(double x, double y);
    void setSize(double width, double height);

    bool contains(double px, double py) const;
    bool contains(const Point& p) const;
    void move(double dx, double dy);
    void resize(double dw, double dh);
    void display(std::ostream& os = std::cout) const;
    
    double getTopLeftX() const { return topLeft.x; }
    double getTopLeftY() const { return topLeft.y; }
    double getBottomRightX() const { return bottomRight.x; }
    double getBottomRightY() const { return bottomRight.y; }
};

} // namespace ppt_cli

#endif // BOUNDING_BOX_H_
