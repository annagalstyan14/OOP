#ifndef SVG_CANVAS_H_
#define SVG_CANVAS_H_

#include "ICanvas.h"
#include <sstream>

namespace ppt {

class SVGCanvas : public ICanvas {
private:
    int width_;
    int height_;
    std::ostringstream content_;

public:
    SVGCanvas(int width = 800, int height = 600) 
        : width_(width), height_(height) {}
    
    int getWidth() const override { return width_; }
    int getHeight() const override { return height_; }
    
    void drawRectangle(const Geometry& geom, const PaintProperties& props) override {
        content_ << "  <rect "
                 << "x=\"" << geom.getX() << "\" "
                 << "y=\"" << geom.getY() << "\" "
                 << "width=\"" << geom.getWidth() << "\" "
                 << "height=\"" << geom.getHeight() << "\" "
                 << "fill=\"" << props.fillColor << "\" "
                 << "stroke=\"" << props.strokeColor << "\" "
                 << "stroke-width=\"" << props.strokeWidth << "\"";
        if (props.opacity < 1.0) {
            content_ << " opacity=\"" << props.opacity << "\"";
        }
        content_ << "/>\n";
    }
    
    void drawEllipse(const Point& center, double rx, double ry, 
                     const PaintProperties& props) override {
        content_ << "  <ellipse "
                 << "cx=\"" << center.x << "\" "
                 << "cy=\"" << center.y << "\" "
                 << "rx=\"" << rx << "\" "
                 << "ry=\"" << ry << "\" "
                 << "fill=\"" << props.fillColor << "\" "
                 << "stroke=\"" << props.strokeColor << "\" "
                 << "stroke-width=\"" << props.strokeWidth << "\"";
        if (props.opacity < 1.0) {
            content_ << " opacity=\"" << props.opacity << "\"";
        }
        content_ << "/>\n";
    }
    
    void drawLine(const Point& start, const Point& end, 
                  const PaintProperties& props) override {
        content_ << "  <line "
                 << "x1=\"" << start.x << "\" "
                 << "y1=\"" << start.y << "\" "
                 << "x2=\"" << end.x << "\" "
                 << "y2=\"" << end.y << "\" "
                 << "stroke=\"" << props.strokeColor << "\" "
                 << "stroke-width=\"" << props.strokeWidth << "\"";
        if (props.strokeStyle == "dashed") {
            content_ << " stroke-dasharray=\"5,5\"";
        } else if (props.strokeStyle == "dotted") {
            content_ << " stroke-dasharray=\"2,2\"";
        }
        content_ << "/>\n";
    }
    
    void drawText(const Point& position, const std::string& text,
                  const std::string& font, int fontSize, 
                  const std::string& color) override {
        content_ << "  <text "
                 << "x=\"" << position.x << "\" "
                 << "y=\"" << position.y << "\" "
                 << "font-family=\"" << font << "\" "
                 << "font-size=\"" << fontSize << "\" "
                 << "fill=\"" << color << "\">"
                 << text << "</text>\n";
    }
    
    std::string getOutput() const override {
        std::ostringstream svg;
        svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            << "width=\"" << width_ << "\" height=\"" << height_ << "\" "
            << "viewBox=\"0 0 " << width_ << " " << height_ << "\">\n";
        svg << "  <rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
        svg << content_.str();
        svg << "</svg>\n";
        return svg.str();
    }
    
    void clear() override {
        content_.str("");
        content_.clear();
    }
};

} // namespace ppt

#endif // SVG_CANVAS_H_
