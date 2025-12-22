#ifndef PPT_SLIDE_H_
#define PPT_SLIDE_H_

#include "SlideObject.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include "TextObject.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

namespace ppt {

class Slide {
private:
    int id_;
    std::string title_;
    std::vector<std::unique_ptr<SlideObject>> objects_;
    int nextObjectId_ = 1;

public:
    explicit Slide(int id, const std::string& title = "Untitled")
        : id_(id), title_(title) {}
    
    Slide(const Slide& other)
        : id_(other.id_)
        , title_(other.title_)
        , nextObjectId_(other.nextObjectId_) {
        for (const auto& obj : other.objects_) {
            objects_.push_back(obj->clone());
        }
    }
    
    Slide& operator=(const Slide& other) {
        if (this != &other) {
            id_ = other.id_;
            title_ = other.title_;
            nextObjectId_ = other.nextObjectId_;
            objects_.clear();
            for (const auto& obj : other.objects_) {
                objects_.push_back(obj->clone());
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
    size_t objectCount() const { return objects_.size(); }
    int getNextObjectId() const { return nextObjectId_; }

    // Setters
    void setId(int id) { id_ = id; }
    void setTitle(const std::string& title) { title_ = title; }
    void setNextObjectId(int id) { nextObjectId_ = id; }

    // Object access
    const std::vector<std::unique_ptr<SlideObject>>& getObjects() const { return objects_; }
    std::vector<std::unique_ptr<SlideObject>>& getObjects() { return objects_; }
    
    SlideObject* getObject(int objectId) {
        for (auto& obj : objects_) {
            if (obj->getId() == objectId) {
                return obj.get();
            }
        }
        return nullptr;
    }
    
    const SlideObject* getObject(int objectId) const {
        for (const auto& obj : objects_) {
            if (obj->getId() == objectId) {
                return obj.get();
            }
        }
        return nullptr;
    }
    
    SlideObject* getObjectAt(size_t index) {
        return index < objects_.size() ? objects_[index].get() : nullptr;
    }
    
    const SlideObject* getObjectAt(size_t index) const {
        return index < objects_.size() ? objects_[index].get() : nullptr;
    }

    // Object operations
    SlideObject* addObject(ObjectType type, const std::string& name, 
                           const std::string& color = "black") {
        std::unique_ptr<SlideObject> obj;
        int id = nextObjectId_++;
        
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
            case ObjectType::TEXT:
                obj = std::make_unique<TextObject>(id, name, "", color);
                break;
        }
        
        objects_.push_back(std::move(obj));
        return objects_.back().get();
    }
    
    SlideObject* addObject(std::unique_ptr<SlideObject> obj) {
        objects_.push_back(std::move(obj));
        return objects_.back().get();
    }
    
    bool removeObject(int objectId) {
        auto it = std::find_if(objects_.begin(), objects_.end(),
                               [objectId](const std::unique_ptr<SlideObject>& o) { 
                                   return o->getId() == objectId; 
                               });
        if (it == objects_.end()) return false;
        objects_.erase(it);
        return true;
    }
    
    bool removeObjectAt(size_t index) {
        if (index >= objects_.size()) return false;
        objects_.erase(objects_.begin() + index);
        return true;
    }

    // Text-specific operations
    TextObject* addText(const std::string& name, const std::string& content, 
                        const std::string& color = "black") {
        int id = nextObjectId_++;
        auto text = std::make_unique<TextObject>(id, name, content, color);
        TextObject* ptr = text.get();
        objects_.push_back(std::move(text));
        return ptr;
    }

    // Display
    void display(std::ostream& os = std::cout) const {
        os << "Slide #" << id_ << ": " << title_ << "\n";
        if (objects_.empty()) {
            os << "  (empty)\n";
        } else {
            os << "  Objects (" << objects_.size() << "):\n";
            for (const auto& obj : objects_) {
                os << "    ";
                obj->display(os);
            }
        }
    }
};

} // namespace ppt

#endif // PPT_SLIDE_H_
