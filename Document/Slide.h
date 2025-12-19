#ifndef SLIDE_H_
#define SLIDE_H_

#include "Elements/IShape.h"
#include "Elements/Shape.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

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
    int id_;
    std::string title_;
    std::vector<std::unique_ptr<IShape>> shapes_;
    std::unordered_map<std::string, TextArea> textAreas_;
    int nextShapeId_ = 1;

public:
    explicit Slide(int id, const std::string& title = "Untitled");
    Slide(const Slide& other);
    Slide& operator=(const Slide& other);
    ~Slide() = default;

    // Getters
    int getId() const { return id_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getText() const { return title_; }
    const std::vector<std::unique_ptr<IShape>>& getShapes() const { return shapes_; }
    std::vector<std::unique_ptr<IShape>>& getShapes() { return shapes_; }
    const std::unordered_map<std::string, TextArea>& getTextAreas() const { return textAreas_; }

    // Setters
    void setId(int newId) { id_ = newId; }
    void setTitle(const std::string& title) { title_ = title; }

    // Shape operations
    IShape* addShape(const std::string& name, ShapeType type = ShapeType::RECTANGLE, 
                     const std::string& color = "black");
    IShape* addShape(std::unique_ptr<IShape> shape);
    IShape* getShape(int shapeId);
    const IShape* getShape(int shapeId) const;
    IShape* getShapeAt(size_t index);
    bool removeShape(int shapeId);
    bool removeShapeAt(size_t index);
    size_t shapeCount() const { return shapes_.size(); }

    // Text operations
    void addText(const std::string& area, const std::string& content);
    void editText(const std::string& area, const std::string& newContent);
    void removeText(const std::string& area);
    void setFont(const std::string& area, const std::string& font, int size, const std::string& color);
    void alignText(const std::string& area, const std::string& alignment);
    bool hasTextArea(const std::string& area) const;

    // Display
    void display(std::ostream& os = std::cout) const;
};

} // namespace ppt_cli

#endif // SLIDE_H_
