#include "BoundingBox.h"

namespace ppt_cli {

BoundingBox::BoundingBox(double x, double y, double w, double h)
    : x(x), y(y), width(w), height(h) {}

bool BoundingBox::contains(double px, double py) const {
    return px >= x && px <= x + width && py >= y && py <= y + height;
}

void BoundingBox::move(double dx, double dy) {
    x += dx;
    y += dy;
}

void BoundingBox::resize(double dw, double dh) {
    width = std::max(1.0, width + dw);
    height = std::max(1.0, height + dh);
}

void BoundingBox::display(std::ostream& os) const {
    os << "(x:" << x << ", y:" << y
       << ", w:" << width << ", h:" << height << ")";
}

} // namespace ppt_cli