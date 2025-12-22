#ifndef JSON_SERIALIZER_H_
#define JSON_SERIALIZER_H_

#include "Presentation.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include "TextObject.h"
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace ppt {

using json = nlohmann::json;

// ============================================================================
// Serializer - Converts Presentation to JSON and saves to file
// ============================================================================
class Serializer {
public:
    static json serializeGeometry(const Geometry& geom) {
        return {
            {"topLeft", {{"x", geom.topLeft.x}, {"y", geom.topLeft.y}}},
            {"bottomRight", {{"x", geom.bottomRight.x}, {"y", geom.bottomRight.y}}}
        };
    }
    
    static json serializeObject(const SlideObject& obj) {
        json o;
        o["id"] = obj.getId();
        o["name"] = obj.getName();
        o["type"] = SlideObject::typeToString(obj.getType());
        o["color"] = obj.getColor();
        o["geometry"] = serializeGeometry(obj.getGeometry());
        
        if (obj.getType() == ObjectType::TEXT) {
            const TextObject* text = dynamic_cast<const TextObject*>(&obj);
            if (text) {
                o["content"] = text->getContent();
                o["fontFamily"] = text->getFontFamily();
                o["fontSize"] = text->getFontSize();
                o["alignment"] = text->getAlignment();
            }
        }
        
        return o;
    }
    
    static json serializeSlide(const Slide& slide) {
        json s;
        s["id"] = slide.getId();
        s["title"] = slide.getTitle();
        s["nextObjectId"] = slide.getNextObjectId();
        s["objects"] = json::array();
        
        for (const auto& obj : slide.getObjects()) {
            s["objects"].push_back(serializeObject(*obj));
        }
        
        return s;
    }
    
    static json serializePresentation(const Presentation& pres) {
        json j;
        j["nextSlideId"] = pres.getNextSlideId();
        j["slides"] = json::array();
        
        for (const auto& slide : pres.getSlides()) {
            j["slides"].push_back(serializeSlide(slide));
        }
        
        return j;
    }
    
    static bool saveToFile(const Presentation& pres, const std::string& filename) {
        std::ofstream file(filename);
        if (!file) return false;
        
        json j = serializePresentation(pres);
        file << j.dump(4);
        return true;
    }
    
    static std::string toString(const Presentation& pres) {
        return serializePresentation(pres).dump(4);
    }
};

// ============================================================================
// Deserializer - Loads JSON and converts to Presentation
// ============================================================================
class Deserializer {
public:
    static Geometry deserializeGeometry(const json& g) {
        return Geometry(
            g["topLeft"].value("x", 0.0),
            g["topLeft"].value("y", 0.0),
            g["bottomRight"].value("x", 100.0),
            g["bottomRight"].value("y", 100.0)
        );
    }
    
    static std::unique_ptr<SlideObject> deserializeObject(const json& o) {
        std::string typeStr = o.value("type", "rectangle");
        ObjectType type = SlideObject::stringToType(typeStr);
        std::string name = o.value("name", "");
        std::string color = o.value("color", "black");
        int id = o.value("id", 1);
        
        std::unique_ptr<SlideObject> obj;
        
        switch (type) {
            case ObjectType::RECTANGLE:
                obj = std::make_unique<Rectangle>(id, name, color);
                break;
            case ObjectType::CIRCLE:
                obj = std::make_unique<Circle>(id, name, color);
                break;
            case ObjectType::LINE:
                obj = std::make_unique<Line>(id, name, color);
                break;
            case ObjectType::TEXT: {
                auto text = std::make_unique<TextObject>(id, name, 
                    o.value("content", ""), color);
                text->setFontFamily(o.value("fontFamily", "Arial"));
                text->setFontSize(o.value("fontSize", 12));
                text->setAlignment(o.value("alignment", "left"));
                obj = std::move(text);
                break;
            }
        }
        
        if (o.contains("geometry")) {
            obj->setGeometry(deserializeGeometry(o["geometry"]));
        }
        
        return obj;
    }
    
    static Slide deserializeSlide(const json& s) {
        Slide slide(s.value("id", 1), s.value("title", "Untitled"));
        slide.setNextObjectId(s.value("nextObjectId", 1));
        
        for (const auto& o : s.value("objects", json::array())) {
            auto obj = deserializeObject(o);
            slide.addObject(std::move(obj));
        }
        
        return slide;
    }
    
    static bool deserializePresentation(Presentation& pres, const json& j) {
        pres.clear();
        pres.setNextSlideId(j.value("nextSlideId", 1));
        
        for (const auto& s : j["slides"]) {
            Slide slide = deserializeSlide(s);
            
            // Add slide and update its properties
            Slide* added = pres.addSlide(slide.getTitle());
            added->setId(slide.getId());
            added->setNextObjectId(slide.getNextObjectId());
            
            // Move objects from deserialized slide
            for (auto& obj : slide.getObjects()) {
                added->addObject(obj->clone());
            }
            
            // Update next slide ID if needed
            if (slide.getId() >= pres.getNextSlideId()) {
                pres.setNextSlideId(slide.getId() + 1);
            }
        }
        
        return true;
    }
    
    static bool loadFromFile(Presentation& pres, const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return false;
        
        json j;
        try {
            file >> j;
        } catch (...) {
            return false;
        }
        
        return deserializePresentation(pres, j);
    }
    
    static bool fromString(Presentation& pres, const std::string& jsonStr) {
        json j;
        try {
            j = json::parse(jsonStr);
        } catch (...) {
            return false;
        }
        
        return deserializePresentation(pres, j);
    }
};

// ============================================================================
// Convenience functions for backward compatibility
// ============================================================================
namespace JSONSerializer {
    inline bool save(const Presentation& pres, const std::string& filename) {
        return Serializer::saveToFile(pres, filename);
    }
    
    inline bool load(Presentation& pres, const std::string& filename) {
        return Deserializer::loadFromFile(pres, filename);
    }
    
    inline void print(const std::string& filename) {
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
}

} // namespace ppt

#endif // JSON_SERIALIZER_H_
