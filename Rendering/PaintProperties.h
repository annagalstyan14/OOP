#ifndef PAINT_PROPERTIES_H_
#define PAINT_PROPERTIES_H_

#include <string>

namespace ppt {

struct PaintProperties {
    std::string fillColor = "white";
    std::string strokeColor = "black";
    double strokeWidth = 1.0;
    double opacity = 1.0;
    std::string strokeStyle = "solid";  // solid, dashed, dotted
    
    PaintProperties() = default;
    
    PaintProperties(const std::string& fill, const std::string& stroke, 
                    double width = 1.0)
        : fillColor(fill), strokeColor(stroke), strokeWidth(width) {}
};

} // namespace ppt

#endif // PAINT_PROPERTIES_H_
