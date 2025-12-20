#ifndef TEXT_AREA_H_
#define TEXT_AREA_H_

#include <string>

namespace ppt_cli {

struct TextArea {
    std::string content;
    std::string fontFamily = "Arial";
    int fontSize = 12;
    std::string color = "black";
    std::string alignment = "left";
    
    TextArea() = default;
    TextArea(const std::string& c) : content(c) {}
};

} // namespace ppt_cli

#endif // TEXT_AREA_H_
