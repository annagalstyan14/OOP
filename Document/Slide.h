#ifndef SLIDE_H_
#define SLIDE_H_

#include "SlideObject.h"
#include <vector>
#include <string>
#include <unordered_map>

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
    explicit Slide(int id, std::string text = "blank");
    Slide(const Slide& other);

    int getId() const;
    void setId(int newId);
    const std::string& getText() const;
    const std::vector<SlideObject>& getObjects() const;

    void addObject(const SlideObject& obj);
    void addText(const std::string& area, const std::string& content);
    void editText(const std::string& area, const std::string& newContent);
    void removeText(const std::string& area);
    void setFont(const std::string& area, const std::string& font, int size, const std::string& color);
    void alignText(const std::string& area, const std::string& alignment);
    void display() const;
};

} // namespace ppt_cli

#endif // SLIDE_H_