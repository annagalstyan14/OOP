#ifndef SLIDE_H_
#define SLIDE_H_

#include "SlideObject.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include "TextObject.h"
#include <vector>
#include <memory>
#include <string>

namespace ppt {

class Slide {
private:
    int id_;
    std::string title_;
    std::vector<std::unique_ptr<SlideObject>> objects_;
    int nextObjectId_ = 1;

public:
    explicit Slide(int id, const std::string& title = "Untitled");
    Slide(const Slide& other);
    Slide& operator=(const Slide& other);
    Slide(Slide&& other) noexcept = default;
    Slide& operator=(Slide&& other) noexcept = default;
    ~Slide() = default;

    // Getters
    int getId() const { return id_; }
    const std::string& getTitle() const { return title_; }
    size_t objectCount() const { return objects_.size(); }
    int getNextObjectId() const { return nextObjectId_; }

    // Setters
    void setId(int id) { id_ = id; }
    void setTitle(const std::string& title) { title_ = title; }
    void setNextObjectId(int id) { nextObjectId_ = id; }

    // Object access
    const std::vector<std::unique_ptr<SlideObject>>& getObjects() const { return objects_; }
    std::vector<std::unique_ptr<SlideObject>>& getObjects() { return objects_; }
    
    SlideObject* getObject(int objectId);
    const SlideObject* getObject(int objectId) const;
    SlideObject* getObjectAt(size_t index);
    const SlideObject* getObjectAt(size_t index) const;

    // Object operations
    SlideObject* addObject(ObjectType type, const std::string& name, 
                           const std::string& color = "black");
    SlideObject* addObject(std::unique_ptr<SlideObject> obj);
    bool removeObject(int objectId);
    bool removeObjectAt(size_t index);

    // Text-specific operations
    TextObject* addText(const std::string& name, const std::string& content, 
                        const std::string& color = "black");

    // Display
    void display(std::ostream& os = std::cout) const;
};

} // namespace ppt

#endif // SLIDE_H_
