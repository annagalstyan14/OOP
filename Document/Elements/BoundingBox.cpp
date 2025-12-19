#include "BoundingBox.h"
#include <algorithm>

namespace ppt_cli {

BoundingBox::BoundingBox(double x1, double y1, double x2, double y2)
    : topLeft(x1, y1), bottomRight(x2, y2) {}

BoundingBox::BoundingBox(const Point& tl, const Point& br)
    : topLeft(tl), bottomRight(br) {}

void BoundingBox::setPosition(double x, double y) {
    double w = getWidth();
    double h = getHeight();
    topLeft.x = x;
    topLeft.y = y;
    bottomRight.x = x + w;
    bottomRight.y = y + h;
}

void BoundingBox::setSize(double width, double height) {
    bottomRight.x = topLeft.x + std::max(1.0, width);
    bottomRight.y = topLeft.y + std::max(1.0, height);
}

bool BoundingBox::contains(double px, double py) const {
    return px >= topLeft.x && px <= bottomRight.x && 
           py >= topLeft.y && py <= bottomRight.y;
}

bool BoundingBox::contains(const Point& p) const {
    return contains(p.x, p.y);
}

void BoundingBox::move(double dx, double dy) {
    topLeft.x += dx;
    topLeft.y += dy;
    bottomRight.x += dx;
    bottomRight.y += dy;
}

void BoundingBox::resize(double dw, double dh) {
    bottomRight.x = topLeft.x + std::max(1.0, getWidth() + dw);
    bottomRight.y = topLeft.y + std::max(1.0, getHeight() + dh);
}

void BoundingBox::display(std::ostream& os) const {
    os << "(topLeft: [" << topLeft.x << ", " << topLeft.y << "], "
       << "bottomRight: [" << bottomRight.x << ", " << bottomRight.y << "])";
}

} // namespace ppt_cli
