#include "Slide.h"
#include <iostream>
#include <stdexcept>

namespace ppt_cli {

    Slide::Slide(int id, std::string text)
    : id(id), text(std::move(text)) {}

    Slide::Slide(const Slide& other)
    : id(other.id), objects(other.objects), text(other.text), textAreas(other.textAreas) {}

int Slide::getId() const { return id; }
void Slide::setId(int newId) { id = newId; }
const std::string& Slide::getText() const { return text; }
const std::vector<SlideObject>& Slide::getObjects() const { return objects; }
const std::unordered_map<std::string, TextArea>& Slide::getTextAreas() const { return textAreas; }

void Slide::addObject(const SlideObject& obj) {
    objects.push_back(obj);
}

void Slide::addText(const std::string& area, const std::string& content) {
    if (textAreas.count(area))
        throw std::runtime_error("Text area '" + area + "' already exists.");
    TextArea ta; ta.content = content;
    textAreas[area] = ta;
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

void Slide::setFont(const std::string& area,
                    const std::string& font,
                    int size,
                    const std::string& color) {
    if (!textAreas.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    auto& ta = textAreas[area];
    if (!font.empty()) ta.fontFamily = font;
    if (size > 0)      ta.fontSize   = size;
    if (!color.empty())ta.color      = color;
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
        for (const auto& [a, ta] : textAreas) {
            std::cout << "  [" << a << "]: \"" << ta.content << "\" "
                      << "(font: " << ta.fontFamily << ", "
                      << ta.fontSize << "pt, " << ta.color << ", "
                      << ta.alignment << ")\n";
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