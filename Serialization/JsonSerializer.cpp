#include "JsonSerializer.h"
#include "../Document/Elements/IShape.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ppt_cli {

bool JsonSerializer::serialize(const Presentation& presentation, const std::string& filename) {
    json j;
    j["nextId"] = presentation.getNextId();
    j["slides"] = json::array();

    for (const auto& slide : presentation.getSlides()) {
        json s;
        s["id"] = slide.getId();
        s["title"] = slide.getTitle();
        s["textAreas"] = json::array();
        s["shapes"] = json::array();

        // Serialize text areas
        for (const auto& [area, text] : slide.getTextAreas()) {
            json t;
            t["area"] = area;
            t["content"] = text.content;
            t["fontFamily"] = text.fontFamily;
            t["fontSize"] = text.fontSize;
            t["color"] = text.color;
            t["alignment"] = text.alignment;
            s["textAreas"].push_back(t);
        }

        // Serialize shapes
        for (const auto& shape : slide.getShapes()) {
            json sh;
            sh["id"] = shape->getId();
            sh["name"] = shape->getName();
            sh["type"] = IShape::shapeTypeToString(shape->getType());
            sh["color"] = shape->getColor();
            
            const BoundingBox& box = shape->getBoundingBox();
            sh["boundingBox"] = {
                {"topLeft", {{"x", box.getTopLeftX()}, {"y", box.getTopLeftY()}}},
                {"bottomRight", {{"x", box.getBottomRightX()}, {"y", box.getBottomRightY()}}}
            };
            
            s["shapes"].push_back(sh);
        }

        j["slides"].push_back(s);
    }

    std::ofstream file(filename);
    if (!file) return false;
    file << j.dump(4);
    return true;
}

} // namespace ppt_cli
