#ifndef PPT_CLI_SLIDE_H_
#define PPT_CLI_SLIDE_H_

#include "Elements/IShape.h"
#include "Elements/TextArea.h"
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <iostream>

namespace ppt_cli {

class Slide {
private:
    int id_;
    std::string title_;
    std::vector<std::unique_ptr<IShape>> shapes_;
    std::map<std::string, TextArea> textAreas_;
    int nextShapeId_ = 1;

public:
    explicit Slide(int id = 0, const std::string& title = "Untitled")
        : id_(id), title_(title) {}
    
    Slide(const Slide& other)
        : id_(other.id_)
        , title_(other.title_)
        , textAreas_(other.textAreas_)
        , nextShapeId_(other.nextShapeId_) {
        for (const auto& shape : other.shapes_) {
            shapes_.push_back(shape->clone());
        }
    }
    
    Slide& operator=(const Slide& other) {
        if (this != &other) {
            id_ = other.id_;
            title_ = other.title_;
            textAreas_ = other.textAreas_;
            nextShapeId_ = other.nextShapeId_;
            shapes_.clear();
            for (const auto& shape : other.shapes_) {
                shapes_.push_back(shape->clone());
            }
        }
        return *this;
    }
    
    Slide(Slide&& other) noexcept = default;
    Slide& operator=(Slide&& other) noexcept = default;
    ~Slide() = default;

    // Getters
    int getId() const { return id_; }
    const std::string& getTitle() const { return title_; }
    size_t shapeCount() const { return shapes_.size(); }
    
    // Setters
    void setId(int id) { id_ = id; }
    void setTitle(const std::string& title) { title_ = title; }

    // Shape access
    const std::vector<std::unique_ptr<IShape>>& getShapes() const { return shapes_; }
    std::vector<std::unique_ptr<IShape>>& getShapes() { return shapes_; }
    
    // Text area access
    const std::map<std::string, TextArea>& getTextAreas() const { return textAreas_; }
    std::map<std::string, TextArea>& getTextAreas() { return textAreas_; }

    // Shape operations
    IShape* addShape(const std::string& name, ShapeType type, const std::string& color = "black") {
        auto shape = std::make_unique<Shape>(nextShapeId_++, name, type, color);
        IShape* ptr = shape.get();
        shapes_.push_back(std::move(shape));
        return ptr;
    }
    
    bool removeShape(int shapeId) {
        auto it = std::find_if(shapes_.begin(), shapes_.end(),
            [shapeId](const std::unique_ptr<IShape>& s) { return s->getId() == shapeId; });
        if (it == shapes_.end()) return false;
        shapes_.erase(it);
        return true;
    }

    // Text operations
    void addText(const std::string& area, const std::string& content) {
        textAreas_[area] = TextArea(content);
    }
    
    void removeText(const std::string& area) {
        textAreas_.erase(area);
    }
    
    void setFont(const std::string& area, const std::string& fontFamily, 
                 int fontSize, const std::string& color) {
        if (textAreas_.find(area) != textAreas_.end()) {
            textAreas_[area].fontFamily = fontFamily;
            textAreas_[area].fontSize = fontSize;
            textAreas_[area].color = color;
        }
    }
    
    void alignText(const std::string& area, const std::string& alignment) {
        if (textAreas_.find(area) != textAreas_.end()) {
            textAreas_[area].alignment = alignment;
        }
    }

    // Display
    void display(std::ostream& os = std::cout) const {
        os << "Slide #" << id_ << ": " << title_ << "\n";
        os << "  Shapes (" << shapes_.size() << "):\n";
        for (const auto& shape : shapes_) {
            os << "    " << IShape::shapeTypeToString(shape->getType()) 
               << " #" << shape->getId() << ": " << shape->getName() << "\n";
        }
        os << "  Text Areas (" << textAreas_.size() << "):\n";
        for (const auto& [area, text] : textAreas_) {
            os << "    [" << area << "]: " << text.content << "\n";
        }
    }
};

} // namespace ppt_cli

#endif // PPT_CLI_SLIDE_H_
