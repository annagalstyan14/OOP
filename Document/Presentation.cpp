// Document/Presentation.cpp
#include "Presentation.h"
#include "Models/Shape.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ppt_cli {

Slide* Presentation::addSlide(const std::string& title) {
    slides_.emplace_back(nextId_++, title);
    return &slides_.back();
}

bool Presentation::removeSlide(int id) {
    auto it = std::find_if(slides_.begin(), slides_.end(),
                           [id](const Slide& s) { return s.getId() == id; });
    if (it == slides_.end()) return false;
    slides_.erase(it);
    return true;
}

Slide* Presentation::getSlide(int id) {
    auto it = std::find_if(slides_.begin(), slides_.end(),
                           [id](const Slide& s) { return s.getId() == id; });
    return it != slides_.end() ? &(*it) : nullptr;
}

Slide* Presentation::getSlideAt(size_t index) {
    return index < slides_.size() ? &slides_[index] : nullptr;
}

const Slide* Presentation::getSlideAt(size_t index) const {
    return index < slides_.size() ? &slides_[index] : nullptr;
}

void Presentation::display() const {
    std::cout << "Presentation (" << slides_.size() << " slides):\n";
    for (const auto& slide : slides_) {
        slide.display();
    }
}

// ------------------- JSON SAVE -------------------
bool Presentation::saveToFile(const std::string& filename) const {
    json j;
    j["nextId"] = nextId_;

    for (const auto& slide : slides_) {
        json s;
        s["id"] = slide.getId();
        s["title"] = slide.getText();

        // Text areas
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

        // Shapes
        for (const auto& obj : slide.getObjects()) {
            if (obj.type == ObjectType::SHAPE) {
                json shape;
                shape["name"] = obj.name;
                shape["color"] = obj.color;
                s["shapes"].push_back(shape);
            }
        }

        j["slides"].push_back(s);
    }

    std::ofstream file(filename);
    if (!file) return false;
    file << j.dump(4);
    return true;
}

// ------------------- JSON LOAD -------------------
bool Presentation::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;

    json j;
    try { file >> j; }
    catch (...) { return false; }

    slides_.clear();
    nextId_ = j.value("nextId", 1);

    for (const auto& s : j["slides"]) {
        int id = s["id"];
        std::string title = s.value("title", "Untitled");
        slides_.emplace_back(id, title);

        Slide& slide = slides_.back();
        if (id >= nextId_) nextId_ = id + 1;

        // Load text areas
        for (const auto& t : s.value("textAreas", json::array())) {
            std::string area = t.value("area", "");
            std::string content = t.value("content", "");
            slide.addText(area, content);
            
            // Set font properties if they exist
            std::string fontFamily = t.value("fontFamily", "Arial");
            int fontSize = t.value("fontSize", 12);
            std::string color = t.value("color", "black");
            slide.setFont(area, fontFamily, fontSize, color);
            
            // Set alignment if it exists
            std::string alignment = t.value("alignment", "left");
            slide.alignText(area, alignment);
        }

        // Load shapes
        for (const auto& shape : s.value("shapes", json::array())) {
            std::string shapeName = shape.value("name", "");
            std::string shapeColor = shape.value("color", "black");
            Shape sh(shapeName, ObjectType::SHAPE, shapeColor);
            slide.addObject(sh.toSlideObject());
        }
    }

    return true;
}

} // namespace ppt_cli