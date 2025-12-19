#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <algorithm>
#include <cmath>

namespace ppt {

struct Point {
    double x = 0.0;
    double y = 0.0;
    
    Point() = default;
    Point(double x, double y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const {
        return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

struct Geometry {
    Point topLeft;
    Point bottomRight;

    Geometry() : topLeft(0, 0), bottomRight(100, 100) {}
    Geometry(double x1, double y1, double x2, double y2)
        : topLeft(x1, y1), bottomRight(x2, y2) {}
    Geometry(const Point& tl, const Point& br)
        : topLeft(tl), bottomRight(br) {}

    double getX() const { return topLeft.x; }
    double getY() const { return topLeft.y; }
    double getWidth() const { return bottomRight.x - topLeft.x; }
    double getHeight() const { return bottomRight.y - topLeft.y; }
    
    void setPosition(double x, double y) {
        double w = getWidth();
        double h = getHeight();
        topLeft = Point(x, y);
        bottomRight = Point(x + w, y + h);
    }
    
    void setSize(double width, double height) {
        bottomRight.x = topLeft.x + std::max(1.0, width);
        bottomRight.y = topLeft.y + std::max(1.0, height);
    }

    bool contains(double px, double py) const {
        return px >= topLeft.x && px <= bottomRight.x && 
               py >= topLeft.y && py <= bottomRight.y;
    }

    void move(double dx, double dy) {
        topLeft.x += dx;
        topLeft.y += dy;
        bottomRight.x += dx;
        bottomRight.y += dy;
    }

    void resize(double dw, double dh) {
        bottomRight.x = topLeft.x + std::max(1.0, getWidth() + dw);
        bottomRight.y = topLeft.y + std::max(1.0, getHeight() + dh);
    }
    
    bool operator==(const Geometry& other) const {
        return topLeft == other.topLeft && bottomRight == other.bottomRight;
    }
};

} // namespace ppt

#endif // GEOMETRY_H_
