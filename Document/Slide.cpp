#include "Slide.h"
#include "Elements/Rectangle.h"
#include "Elements/Ellipse.h"
#include "Elements/Triangle.h"
#include "Elements/Line.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace ppt_cli {

Slide::Slide(int id, const std::string& title)
    : id_(id), title_(title) {}

Slide::Slide(const Slide& other)
    : id_(other.id_)
    , title_(other.title_)
    , textAreas_(other.textAreas_)
    , nextShapeId_(other.nextShapeId_) {
    for (const auto& shape : other.shapes_) {
        shapes_.push_back(std::unique_ptr<IShape>(shape->clone()));
    }
}

Slide& Slide::operator=(const Slide& other) {
    if (this != &other) {
        id_ = other.id_;
        title_ = other.title_;
        textAreas_ = other.textAreas_;
        nextShapeId_ = other.nextShapeId_;
        shapes_.clear();
        for (const auto& shape : other.shapes_) {
            shapes_.push_back(std::unique_ptr<IShape>(shape->clone()));
        }
    }
    return *this;
}

IShape* Slide::addShape(const std::string& name, ShapeType type, const std::string& color) {
    std::unique_ptr<IShape> shape;
    int id = nextShapeId_++;
    
    switch (type) {
        case ShapeType::RECTANGLE:
            shape = std::make_unique<Rectangle>(id, name, color);
            break;
        case ShapeType::ELLIPSE:
            shape = std::make_unique<Ellipse>(id, name, color);
            break;
        case ShapeType::TRIANGLE:
            shape = std::make_unique<Triangle>(id, name, color);
            break;
        case ShapeType::LINE:
            shape = std::make_unique<Line>(id, name, color);
            break;
        default:
            shape = std::make_unique<Rectangle>(id, name, color);
    }
    
    shapes_.push_back(std::move(shape));
    return shapes_.back().get();
}

IShape* Slide::addShape(std::unique_ptr<IShape> shape) {
    shapes_.push_back(std::move(shape));
    return shapes_.back().get();
}

IShape* Slide::getShape(int shapeId) {
    for (auto& shape : shapes_) {
        if (shape->getId() == shapeId) {
            return shape.get();
        }
    }
    return nullptr;
}

const IShape* Slide::getShape(int shapeId) const {
    for (const auto& shape : shapes_) {
        if (shape->getId() == shapeId) {
            return shape.get();
        }
    }
    return nullptr;
}

IShape* Slide::getShapeAt(size_t index) {
    return index < shapes_.size() ? shapes_[index].get() : nullptr;
}

bool Slide::removeShape(int shapeId) {
    auto it = std::find_if(shapes_.begin(), shapes_.end(),
                           [shapeId](const std::unique_ptr<IShape>& s) { 
                               return s->getId() == shapeId; 
                           });
    if (it == shapes_.end()) return false;
    shapes_.erase(it);
    return true;
}

bool Slide::removeShapeAt(size_t index) {
    if (index >= shapes_.size()) return false;
    shapes_.erase(shapes_.begin() + index);
    return true;
}

void Slide::addText(const std::string& area, const std::string& content) {
    if (textAreas_.count(area))
        throw std::runtime_error("Text area '" + area + "' already exists.");
    TextArea ta;
    ta.content = content;
    textAreas_[area] = ta;
}

void Slide::editText(const std::string& area, const std::string& newContent) {
    if (!textAreas_.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    textAreas_[area].content = newContent;
}

void Slide::removeText(const std::string& area) {
    if (!textAreas_.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    textAreas_.erase(area);
}

void Slide::setFont(const std::string& area, const std::string& font, int size, const std::string& color) {
    if (!textAreas_.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    auto& ta = textAreas_[area];
    if (!font.empty()) ta.fontFamily = font;
    if (size > 0) ta.fontSize = size;
    if (!color.empty()) ta.color = color;
}

void Slide::alignText(const std::string& area, const std::string& alignment) {
    if (!textAreas_.count(area))
        throw std::runtime_error("Text area '" + area + "' does not exist.");
    textAreas_[area].alignment = alignment;
}

bool Slide::hasTextArea(const std::string& area) const {
    return textAreas_.count(area) > 0;
}

void Slide::display(std::ostream& os) const {
    os << "Slide #" << id_ << " (" << title_ << ")\n";
    
    if (!textAreas_.empty()) {
        os << "  Text Areas:\n";
        for (const auto& [area, ta] : textAreas_) {
            os << "    [" << area << "]: \"" << ta.content << "\" "
               << "(font: " << ta.fontFamily << ", "
               << ta.fontSize << "pt, " << ta.color << ", "
               << ta.alignment << ")\n";
        }
    }
    
    if (!shapes_.empty()) {
        os << "  Shapes (" << shapes_.size() << "):\n";
        for (const auto& shape : shapes_) {
            os << "    ";
            shape->display(os);
        }
    }
    
    if (textAreas_.empty() && shapes_.empty()) {
        os << "  (empty)\n";
    }
}

} // namespace ppt_cli
