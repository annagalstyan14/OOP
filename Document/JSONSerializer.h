#ifndef JSON_SERIALIZER_H_
#define JSON_SERIALIZER_H_

#include "Presentation.h"
#include "TextObject.h"
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace ppt {

using json = nlohmann::json;

class JSONSerializer {
public:
    static bool save(const Presentation& pres, const std::string& filename) {
        json j;
        j["nextSlideId"] = pres.getNextSlideId();
        j["slides"] = json::array();

        for (const auto& slide : pres.getSlides()) {
            json s;
            s["id"] = slide.getId();
            s["title"] = slide.getTitle();
            s["nextObjectId"] = slide.getNextObjectId();
            s["objects"] = json::array();

            for (const auto& obj : slide.getObjects()) {
                json o;
                o["id"] = obj->getId();
                o["name"] = obj->getName();
                o["type"] = SlideObject::typeToString(obj->getType());
                o["color"] = obj->getColor();
                
                const Geometry& g = obj->getGeometry();
                o["geometry"] = {
                    {"topLeft", {{"x", g.topLeft.x}, {"y", g.topLeft.y}}},
                    {"bottomRight", {{"x", g.bottomRight.x}, {"y", g.bottomRight.y}}}
                };
                
                if (obj->getType() == ObjectType::TEXT) {
                    const TextObject* text = dynamic_cast<const TextObject*>(obj.get());
                    if (text) {
                        o["content"] = text->getContent();
                        o["fontFamily"] = text->getFontFamily();
                        o["fontSize"] = text->getFontSize();
                        o["alignment"] = text->getAlignment();
                    }
                }
                
                s["objects"].push_back(o);
            }

            j["slides"].push_back(s);
        }

        std::ofstream file(filename);
        if (!file) return false;
        file << j.dump(4);
        return true;
    }

    static bool load(Presentation& pres, const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return false;

        json j;
        try {
            file >> j;
        } catch (...) {
            return false;
        }

        pres.clear();
        pres.setNextSlideId(j.value("nextSlideId", 1));

        for (const auto& s : j["slides"]) {
            Slide* slide = pres.addSlide(s.value("title", "Untitled"));
            slide->setId(s["id"]);
            slide->setNextObjectId(s.value("nextObjectId", 1));
            
            if (slide->getId() >= pres.getNextSlideId()) {
                pres.setNextSlideId(slide->getId() + 1);
            }

            for (const auto& o : s.value("objects", json::array())) {
                std::string typeStr = o.value("type", "rectangle");
                ObjectType type = SlideObject::stringToType(typeStr);
                std::string name = o.value("name", "");
                std::string color = o.value("color", "black");
                
                SlideObject* obj = slide->addObject(type, name, color);
                obj->setId(o.value("id", obj->getId()));
                
                if (o.contains("geometry")) {
                    const auto& g = o["geometry"];
                    Geometry geom(
                        g["topLeft"].value("x", 0.0),
                        g["topLeft"].value("y", 0.0),
                        g["bottomRight"].value("x", 100.0),
                        g["bottomRight"].value("y", 100.0)
                    );
                    obj->setGeometry(geom);
                }
                
                if (type == ObjectType::TEXT) {
                    TextObject* text = dynamic_cast<TextObject*>(obj);
                    if (text) {
                        text->setContent(o.value("content", ""));
                        text->setFontFamily(o.value("fontFamily", "Arial"));
                        text->setFontSize(o.value("fontSize", 12));
                        text->setAlignment(o.value("alignment", "left"));
                    }
                }
            }
        }

        return true;
    }

    static void print(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "Error: Could not open file " << filename << "\n";
            return;
        }

        json j;
        try {
            file >> j;
            std::cout << "=== " << filename << " ===\n";
            std::cout << j.dump(4) << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error parsing JSON: " << e.what() << "\n";
        }
    }
};

} // namespace ppt

#endif // JSON_SERIALIZER_H_
