#ifndef IACTION_H_
#define IACTION_H_

#include "../Document/Presentation.h"

namespace ppt_cli {

class IAction {
public:
    virtual ~IAction() = default;
    
    // Execute the action
    virtual void execute(Presentation& presentation) = 0;
    
    // Undo the action
    virtual void undo(Presentation& presentation) = 0;
    
    // Check if this action can be undone
    virtual bool isUndoable() const { return true; }
    
    // Get action description for display
    virtual std::string getDescription() const = 0;
};

} // namespace ppt_cli

#endif // IACTION_H_
