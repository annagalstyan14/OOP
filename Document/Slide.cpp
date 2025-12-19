#include "Slide.h"
#include <algorithm>
#include <stdexcept>

namespace ppt {

Slide::Slide(int id, const std::string& title)
    : id_(id), title_(title) {}

Slide::Slide(const Slide& other)
    : id_(other.id_)
    , title_(other.title_)
    , nextObjectId_(other.nextObjectId_) {
    for (const auto& obj : other.objects_) {
        objects_.push_back(obj->clone());
    }
}

Slide& Slide::operator=(const Slide& other) {
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

SlideObject* Slide::getObject(int objectId) {
    for (auto& obj : objects_) {
        if (obj->getId() == objectId) {
            return obj.get();
        }
    }
    return nullptr;
}

const SlideObject* Slide::getObject(int objectId) const {
    for (const auto& obj : objects_) {
        if (obj->getId() == objectId) {
            return obj.get();
        }
    }
    return nullptr;
}

SlideObject* Slide::getObjectAt(size_t index) {
    return index < objects_.size() ? objects_[index].get() : nullptr;
}

const SlideObject* Slide::getObjectAt(size_t index) const {
    return index < objects_.size() ? objects_[index].get() : nullptr;
}

SlideObject* Slide::addObject(ObjectType type, const std::string& name, 
                               const std::string& color) {
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

SlideObject* Slide::addObject(std::unique_ptr<SlideObject> obj) {
    objects_.push_back(std::move(obj));
    return objects_.back().get();
}

bool Slide::removeObject(int objectId) {
    auto it = std::find_if(objects_.begin(), objects_.end(),
                           [objectId](const std::unique_ptr<SlideObject>& o) { 
                               return o->getId() == objectId; 
                           });
    if (it == objects_.end()) return false;
    objects_.erase(it);
    return true;
}

bool Slide::removeObjectAt(size_t index) {
    if (index >= objects_.size()) return false;
    objects_.erase(objects_.begin() + index);
    return true;
}

TextObject* Slide::addText(const std::string& name, const std::string& content, 
                            const std::string& color) {
    int id = nextObjectId_++;
    auto text = std::make_unique<TextObject>(id, name, content, color);
    TextObject* ptr = text.get();
    objects_.push_back(std::move(text));
    return ptr;
}

void Slide::display(std::ostream& os) const {
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

} // namespace ppt
