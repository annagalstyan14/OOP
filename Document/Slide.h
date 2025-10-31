// Document/Slide.h
#ifndef SLIDE_H_
#define SLIDE_H_

#include "SlideObject.h"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>

namespace ppt_cli {

struct TextArea {
    std::string content;
    std::string fontFamily = "Arial";
    int fontSize = 12;
    std::string color = "black";
    std::string alignment = "left";
};

class Slide {
private:
    int id;
    std::vector<SlideObject> objects;
    std::string text;
    std::unordered_map<std::string, TextArea> textAreas;

public:

    explicit Slide(int id, std::string text = "blank")
        : id(id), text(std::move(text)) {}

    Slide(const Slide& other)
        : id(other.id), text(other.text), objects(other.objects), textAreas(other.textAreas) {}

    int getId() const { return id; }
    const std::string& getText() const { return text; }
    const std::vector<SlideObject>& getObjects() const { return objects; }

    void setId(int newId) { id = newId; }

    void addObject(const SlideObject& obj) {
        objects.push_back(obj);
    }

    void addText(const std::string& area, const std::string& content) {
        if (textAreas.count(area))
            throw std::runtime_error("Text area '" + area + "' already exists. Use edit to modify.");
        TextArea newArea;
        newArea.content = content;
        textAreas[area] = newArea;
    }

    void editText(const std::string& area, const std::string& newContent) {
        if (!textAreas.count(area))
            throw std::runtime_error("Text area '" + area + "' does not exist.");
        textAreas[area].content = newContent;
    }

    void removeText(const std::string& area) {
        if (!textAreas.count(area))
            throw std::runtime_error("Text area '" + area + "' does not exist.");
        textAreas.erase(area);
    }

    void setFont(const std::string& area, const std::string& font, int size, const std::string& color) {
        if (!textAreas.count(area))
            throw std::runtime_error("Text area '" + area + "' does not exist.");
        if (!font.empty())
            textAreas[area].fontFamily = font;
        if (size > 0)
            textAreas[area].fontSize = size;
        if (!color.empty())
            textAreas[area].color = color;
    }

    void alignText(const std::string& area, const std::string& alignment) {
        if (!textAreas.count(area))
            throw std::runtime_error("Text area '" + area + "' does not exist.");
        textAreas[area].alignment = alignment;
    }

    void display() const {
        std::cout << "Slide #" << id << " (" << text << ")\n";
        if (!textAreas.empty()) {
            std::cout << " Text Areas:\n";
            for (const auto& [area, textArea] : textAreas) {
                std::cout << "  [" << area << "]: \"" << textArea.content << "\" "
                          << "(font: " << textArea.fontFamily << ", "
                          << textArea.fontSize << "pt, "
                          << textArea.color << ", "
                          << textArea.alignment << ")\n";
            }
        }
        if (!objects.empty()) {
            std::cout << " Objects:\n";
            for (const auto& obj : objects) {
                std::cout << "  - " << obj.name
                          << " (Type: " << static_cast<int>(obj.type)
                          << ", Color: " << obj.color << ")\n";
            }
        }
        if (textAreas.empty() && objects.empty())
            std::cout << " (empty)\n";
    }
};

} // namespace ppt_cli

#endif // SLIDE_H_