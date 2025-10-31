#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include <string>
#include <iostream>

namespace ppt_cli {

struct BoundingBox {
    double x = 0.0;
    double y = 0.0;
    double width = 100.0;
    double height = 100.0;

    BoundingBox() = default;
    BoundingBox(double x, double y, double w, double h)
        : x(x), y(y), width(w), height(h) {}

    bool contains(double px, double py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }

    void resize(double dw, double dh) {
        width = std::max(1.0, width + dw);
        height = std::max(1.0, height + dh);
    }

    void display(std::ostream& os = std::cout) const {
        os << "(x:" << x << ", y:" << y
           << ", w:" << width << ", h:" << height << ")";
    }
};

} // namespace ppt_cli

#endif // BOUNDING_BOX_H