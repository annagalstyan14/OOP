#ifndef IACTION_H_
#define IACTION_H_

#include "../Document/Presentation.h"
#include <string>

namespace ppt {

class IAction {
public:
    virtual ~IAction() = default;
    
    virtual void execute(Presentation& pres) = 0;
    virtual void undo(Presentation& pres) = 0;
    
    virtual bool isUndoable() const { return true; }
    virtual std::string getDescription() const = 0;
};

} // namespace ppt

#endif // IACTION_H_
