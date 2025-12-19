#ifndef EDITOR_H_
#define EDITOR_H_

#include "IAction.h"
#include "../Document/Presentation.h"
#include <stack>
#include <memory>

namespace ppt_cli {

class Editor {
public:
    explicit Editor(Presentation& presentation);
    
    // Execute an action and add to undo stack
    void executeAction(std::unique_ptr<IAction> action);
    
    // Undo/Redo operations
    bool undo();
    bool redo();
    
    // Check if undo/redo available
    bool canUndo() const { return !undoStack_.empty(); }
    bool canRedo() const { return !redoStack_.empty(); }
    
    // Get stack sizes
    size_t undoStackSize() const { return undoStack_.size(); }
    size_t redoStackSize() const { return redoStack_.size(); }
    
    // Clear history
    void clearHistory();

private:
    Presentation& presentation_;
    std::stack<std::unique_ptr<IAction>> undoStack_;
    std::stack<std::unique_ptr<IAction>> redoStack_;
};

} // namespace ppt_cli

#endif // EDITOR_H_
