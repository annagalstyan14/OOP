#include "SvgShapeRenderer.h"
#include <sstream>

namespace ppt_cli {

std::string SvgShapeRenderer::render(const IShape& shape) const {
    switch (shape.getType()) {
        case ShapeType::RECTANGLE:
            return renderRectangle(shape);
        case ShapeType::ELLIPSE:
            return renderEllipse(shape);
        case ShapeType::TRIANGLE:
            return renderTriangle(shape);
        case ShapeType::LINE:
            return renderLine(shape);
        default:
            return renderRectangle(shape);
    }
}

std::string SvgShapeRenderer::renderRectangle(const IShape& shape) const {
    const BoundingBox& box = shape.getBoundingBox();
    std::ostringstream svg;
    
    svg << "  <rect "
        << "x=\"" << box.getX() << "\" "
        << "y=\"" << box.getY() << "\" "
        << "width=\"" << box.getWidth() << "\" "
        << "height=\"" << box.getHeight() << "\" "
        << "fill=\"" << shape.getColor() << "\" "
        << "stroke=\"black\" stroke-width=\"1\"/>\n";
    
    svg << "  <text x=\"" << (box.getX() + box.getWidth() / 2) << "\" "
        << "y=\"" << (box.getY() + box.getHeight() / 2 + 5) << "\" "
        << "text-anchor=\"middle\" font-size=\"12\" fill=\"white\">"
        << shape.getName() << "</text>\n";
    
    return svg.str();
}

std::string SvgShapeRenderer::renderEllipse(const IShape& shape) const {
    const BoundingBox& box = shape.getBoundingBox();
    double cx = box.getX() + box.getWidth() / 2;
    double cy = box.getY() + box.getHeight() / 2;
    double rx = box.getWidth() / 2;
    double ry = box.getHeight() / 2;
    
    std::ostringstream svg;
    svg << "  <ellipse "
        << "cx=\"" << cx << "\" "
        << "cy=\"" << cy << "\" "
        << "rx=\"" << rx << "\" "
        << "ry=\"" << ry << "\" "
        << "fill=\"" << shape.getColor() << "\" "
        << "stroke=\"black\" stroke-width=\"1\"/>\n";
    
    svg << "  <text x=\"" << cx << "\" y=\"" << (cy + 5) << "\" "
        << "text-anchor=\"middle\" font-size=\"12\" fill=\"white\">"
        << shape.getName() << "</text>\n";
    
    return svg.str();
}

std::string SvgShapeRenderer::renderTriangle(const IShape& shape) const {
    const BoundingBox& box = shape.getBoundingBox();
    
    double x1 = box.getX() + box.getWidth() / 2;
    double y1 = box.getY();
    double x2 = box.getX();
    double y2 = box.getY() + box.getHeight();
    double x3 = box.getX() + box.getWidth();
    double y3 = box.getY() + box.getHeight();
    
    std::ostringstream svg;
    svg << "  <polygon "
        << "points=\"" << x1 << "," << y1 << " " 
        << x2 << "," << y2 << " " 
        << x3 << "," << y3 << "\" "
        << "fill=\"" << shape.getColor() << "\" "
        << "stroke=\"black\" stroke-width=\"1\"/>\n";
    
    double cx = box.getX() + box.getWidth() / 2;
    double cy = box.getY() + box.getHeight() * 0.6;
    svg << "  <text x=\"" << cx << "\" y=\"" << cy << "\" "
        << "text-anchor=\"middle\" font-size=\"12\" fill=\"white\">"
        << shape.getName() << "</text>\n";
    
    return svg.str();
}

std::string SvgShapeRenderer::renderLine(const IShape& shape) const {
    const BoundingBox& box = shape.getBoundingBox();
    
    std::ostringstream svg;
    svg << "  <line "
        << "x1=\"" << box.topLeft.x << "\" "
        << "y1=\"" << box.topLeft.y << "\" "
        << "x2=\"" << box.bottomRight.x << "\" "
        << "y2=\"" << box.bottomRight.y << "\" "
        << "stroke=\"" << shape.getColor() << "\" "
        << "stroke-width=\"2\"/>\n";
    
    return svg.str();
}

} // namespace ppt_cli
