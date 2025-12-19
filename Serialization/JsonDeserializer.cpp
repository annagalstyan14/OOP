#include "JsonDeserializer.h"
#include "../Document/Elements/IShape.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ppt_cli {

bool JsonDeserializer::deserialize(Presentation& presentation, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;

    json j;
    try {
        file >> j;
    } catch (...) {
        return false;
    }

    presentation.getSlides().clear();
    presentation.setNextId(j.value("nextId", 1));

    for (const auto& s : j["slides"]) {
        int id = s["id"];
        std::string title = s.value("title", "Untitled");
        
        Slide* slide = presentation.addSlide(title);
        slide->setId(id);
        
        if (id >= presentation.getNextId()) {
            presentation.setNextId(id + 1);
        }

        // Load text areas
        for (const auto& t : s.value("textAreas", json::array())) {
            std::string area = t.value("area", "");
            std::string content = t.value("content", "");
            slide->addText(area, content);

            std::string fontFamily = t.value("fontFamily", "Arial");
            int fontSize = t.value("fontSize", 12);
            std::string color = t.value("color", "black");
            slide->setFont(area, fontFamily, fontSize, color);

            std::string alignment = t.value("alignment", "left");
            slide->alignText(area, alignment);
        }

        // Load shapes
        for (const auto& sh : s.value("shapes", json::array())) {
            std::string name = sh.value("name", "");
            std::string typeStr = sh.value("type", "rectangle");
            std::string color = sh.value("color", "black");
            
            IShape* shape = slide->addShape(name, IShape::stringToShapeType(typeStr), color);
            
            if (sh.contains("boundingBox")) {
                const auto& box = sh["boundingBox"];
                double x1 = box["topLeft"].value("x", 0.0);
                double y1 = box["topLeft"].value("y", 0.0);
                double x2 = box["bottomRight"].value("x", 100.0);
                double y2 = box["bottomRight"].value("y", 100.0);
                shape->setBoundingBox(BoundingBox(x1, y1, x2, y2));
            }
            
            if (sh.contains("id")) {
                shape->setId(sh["id"]);
            }
        }
    }

    return true;
}

void JsonDeserializer::printToConsole(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Error: Could not open file " << filename << "\n";
        return;
    }

    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        std::cout << "Error parsing JSON: " << e.what() << "\n";
        return;
    }

    std::cout << "=== Deserialized Presentation from " << filename << " ===\n\n";
    std::cout << j.dump(4) << "\n";
    std::cout << "\n=== End of JSON ===\n";
}

} // namespace ppt_cli
