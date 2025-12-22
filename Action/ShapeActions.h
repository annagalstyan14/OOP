#ifndef SHAPE_ACTIONS_H_
#define SHAPE_ACTIONS_H_

#include "IAction.h"
#include <iostream>
#include <memory>

namespace ppt {

class AddShapeAction : public IAction {
private:
    std::string name_;
    ObjectType type_;
    std::string color_;
    int addedObjectId_ = -1;
    size_t slideIndex_ = 0;

public:
    AddShapeAction(const std::string& name, ObjectType type, const std::string& color = "black")
        : name_(name), type_(type), color_(color) {}
    
    void execute(Presentation& pres) override {
        if (pres.empty()) {
            throw std::runtime_error("No slide to add shape to.");
        }
        
        slideIndex_ = pres.size() - 1;
        Slide* slide = pres.getSlideAt(slideIndex_);
        SlideObject* obj = slide->addObject(type_, name_, color_);
        addedObjectId_ = obj->getId();
        
        std::cout << "Added " << SlideObject::typeToString(type_) 
                  << " '" << name_ << "' (id: " << addedObjectId_ << ")\n";
    }
    
    void undo(Presentation& pres) override {
        if (addedObjectId_ >= 0 && slideIndex_ < pres.size()) {
            Slide* slide = pres.getSlideAt(slideIndex_);
            if (slide) {
                slide->removeObject(addedObjectId_);
                std::cout << "Undone: removed " << SlideObject::typeToString(type_) 
                          << " #" << addedObjectId_ << "\n";
            }
        }
    }
    
    std::string getDescription() const override {
        return "Add " + SlideObject::typeToString(type_) + ": " + name_;
    }
};

class RemoveShapeAction : public IAction {
private:
    int objectId_;
    std::unique_ptr<SlideObject> removedObject_;
    size_t slideIndex_ = 0;

public:
    explicit RemoveShapeAction(int objectId) : objectId_(objectId) {}
    
    void execute(Presentation& pres) override {
        if (pres.empty()) {
            throw std::runtime_error("No slide to remove shape from.");
        }
        
        slideIndex_ = pres.size() - 1;
        Slide* slide = pres.getSlideAt(slideIndex_);
        
        // Clone object before removing
        const SlideObject* obj = slide->getObject(objectId_);
        if (obj) {
            removedObject_ = obj->clone();
        }
        
        if (!slide->removeObject(objectId_)) {
            throw std::runtime_error("Shape not found: " + std::to_string(objectId_));
        }
        
        std::cout << "Removed shape #" << objectId_ << "\n";
    }
    
    void undo(Presentation& pres) override {
        if (removedObject_ && slideIndex_ < pres.size()) {
            Slide* slide = pres.getSlideAt(slideIndex_);
            if (slide) {
                slide->addObject(removedObject_->clone());
                std::cout << "Undone: restored shape '" << removedObject_->getName() << "'\n";
            }
        }
    }
    
    std::string getDescription() const override {
        return "Remove shape #" + std::to_string(objectId_);
    }
};

class AddTextAction : public IAction {
private:
    std::string name_;
    std::string content_;
    std::string color_;
    int addedObjectId_ = -1;
    size_t slideIndex_ = 0;

public:
    AddTextAction(const std::string& name, const std::string& content, 
                  const std::string& color = "black")
        : name_(name), content_(content), color_(color) {}
    
    void execute(Presentation& pres) override {
        if (pres.empty()) {
            throw std::runtime_error("No slide to add text to.");
        }
        
        slideIndex_ = pres.size() - 1;
        Slide* slide = pres.getSlideAt(slideIndex_);
        TextObject* text = slide->addText(name_, content_, color_);
        addedObjectId_ = text->getId();
        
        std::cout << "Added text '" << name_ << "' (id: " << addedObjectId_ << ")\n";
    }
    
    void undo(Presentation& pres) override {
        if (addedObjectId_ >= 0 && slideIndex_ < pres.size()) {
            Slide* slide = pres.getSlideAt(slideIndex_);
            if (slide) {
                slide->removeObject(addedObjectId_);
                std::cout << "Undone: removed text #" << addedObjectId_ << "\n";
            }
        }
    }
    
    std::string getDescription() const override {
        return "Add text: " + name_;
    }
};

} // namespace ppt

#endif // SHAPE_ACTIONS_H_
