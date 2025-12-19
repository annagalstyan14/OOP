#include "Editor.h"
#include <iostream>

namespace ppt_cli {

Editor::Editor(Presentation& presentation)
    : presentation_(presentation) {}

void Editor::executeAction(std::unique_ptr<IAction> action) {
    action->execute(presentation_);
    
    if (action->isUndoable()) {
        undoStack_.push(std::move(action));
        
        // Clear redo stack when new action is executed
        while (!redoStack_.empty()) {
            redoStack_.pop();
        }
    }
}

bool Editor::undo() {
    if (undoStack_.empty()) {
        std::cout << "Nothing to undo.\n";
        return false;
    }
    
    auto action = std::move(undoStack_.top());
    undoStack_.pop();
    
    action->undo(presentation_);
    redoStack_.push(std::move(action));
    
    std::cout << "Undo successful.\n";
    return true;
}

bool Editor::redo() {
    if (redoStack_.empty()) {
        std::cout << "Nothing to redo.\n";
        return false;
    }
    
    auto action = std::move(redoStack_.top());
    redoStack_.pop();
    
    action->execute(presentation_);
    undoStack_.push(std::move(action));
    
    std::cout << "Redo successful.\n";
    return true;
}

void Editor::clearHistory() {
    while (!undoStack_.empty()) {
        undoStack_.pop();
    }
    while (!redoStack_.empty()) {
        redoStack_.pop();
    }
}

} // namespace ppt_cli
