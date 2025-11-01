#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include <iostream>

namespace ppt_cli {

struct BoundingBox {
    double x = 0.0;
    double y = 0.0;
    double width = 100.0;
    double height = 100.0;

    BoundingBox() = default;
    BoundingBox(double x, double y, double w, double h);

    bool contains(double px, double py) const;
    void move(double dx, double dy);
    void resize(double dw, double dh);
    void display(std::ostream& os = std::cout) const;
};

} // namespace ppt_cli

#endif // BOUNDING_BOX_H_