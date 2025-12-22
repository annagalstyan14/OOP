#ifndef EDITOR_H_
#define EDITOR_H_

#include "IAction.h"
#include <stack>
#include <memory>
#include <iostream>

namespace ppt {

class Editor {
private:
    Presentation& presentation_;
    std::stack<std::unique_ptr<IAction>> undoStack_;
    std::stack<std::unique_ptr<IAction>> redoStack_;

public:
    explicit Editor(Presentation& pres) : presentation_(pres) {}
    
    void executeAction(std::unique_ptr<IAction> action) {
        action->execute(presentation_);
        
        if (action->isUndoable()) {
            undoStack_.push(std::move(action));
            
            // Clear redo stack on new action
            while (!redoStack_.empty()) {
                redoStack_.pop();
            }
        }
    }
    
    bool undo() {
        if (undoStack_.empty()) {
            std::cout << "Nothing to undo.\n";
            return false;
        }
        
        auto action = std::move(undoStack_.top());
        undoStack_.pop();
        
        action->undo(presentation_);
        redoStack_.push(std::move(action));
        
        return true;
    }
    
    bool redo() {
        if (redoStack_.empty()) {
            std::cout << "Nothing to redo.\n";
            return false;
        }
        
        auto action = std::move(redoStack_.top());
        redoStack_.pop();
        
        action->execute(presentation_);
        undoStack_.push(std::move(action));
        
        return true;
    }
    
    bool canUndo() const { return !undoStack_.empty(); }
    bool canRedo() const { return !redoStack_.empty(); }
    
    size_t undoStackSize() const { return undoStack_.size(); }
    size_t redoStackSize() const { return redoStack_.size(); }
    
    void clearHistory() {
        while (!undoStack_.empty()) undoStack_.pop();
        while (!redoStack_.empty()) redoStack_.pop();
    }
    
    Presentation& getPresentation() { return presentation_; }
    const Presentation& getPresentation() const { return presentation_; }
};

} // namespace ppt

#endif // EDITOR_H_
