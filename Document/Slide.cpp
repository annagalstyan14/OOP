#include "Slide.h"
#include "SlideObject.h"
#include <stdexcept>
#include <iostream>

namespace ppt_cli {


void Slide::addObject(const SlideObject& obj) {
    objects.push_back(obj);
}

void Slide::addText(const std::string& area, const std::string& content) {
    if (textAreas.count(area))
        throw std::runtime_error("Text area '" + area + "' already exists. Use edit to modify.");
    TextArea newArea;
    newArea.content = content;
    textAreas[area] = newArea;
}

void Slide::editText(const std::string& area, const std::string& newContent) {
    if (!textAreas.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    textAreas[area].content = newContent;
}

void Slide::removeText(const std::string& area) {
    if (!textAreas.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    textAreas.erase(area);
}

void Slide::setFont(const std::string& area, const std::string& font, int size, const std::string& color) {
    if (!textAreas.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    if (!font.empty())
        textAreas[area].fontFamily = font;
    if (size > 0)
        textAreas[area].fontSize = size;
    if (!color.empty())
        textAreas[area].color = color;
}

void Slide::alignText(const std::string& area, const std::string& alignment) {
    if (!textAreas.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    textAreas[area].alignment = alignment;
}

void Slide::display() const {
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

} // namespace ppt_cli