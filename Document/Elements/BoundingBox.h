#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

namespace ppt_cli {

struct Point {
    double x = 0.0;
    double y = 0.0;
    
    Point() = default;
    Point(double x_, double y_) : x(x_), y(y_) {}
};

class BoundingBox {
public:
    Point topLeft;
    Point bottomRight;

    BoundingBox() : topLeft(0, 0), bottomRight(100, 100) {}
    
    BoundingBox(double x1, double y1, double x2, double y2)
        : topLeft(x1, y1), bottomRight(x2, y2) {}
    
    BoundingBox(const Point& tl, const Point& br)
        : topLeft(tl), bottomRight(br) {}

    double getX() const { return topLeft.x; }
    double getY() const { return topLeft.y; }
    double getWidth() const { return bottomRight.x - topLeft.x; }
    double getHeight() const { return bottomRight.y - topLeft.y; }
    
    double getTopLeftX() const { return topLeft.x; }
    double getTopLeftY() const { return topLeft.y; }
    double getBottomRightX() const { return bottomRight.x; }
    double getBottomRightY() const { return bottomRight.y; }
    
    void setPosition(double x, double y) {
        double w = getWidth();
        double h = getHeight();
        topLeft = Point(x, y);
        bottomRight = Point(x + w, y + h);
    }
    
    void setSize(double width, double height) {
        bottomRight.x = topLeft.x + width;
        bottomRight.y = topLeft.y + height;
    }
};

} // namespace ppt_cli

#endif // BOUNDING_BOX_H_
