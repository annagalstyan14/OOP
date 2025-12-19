#ifndef ADD_SLIDE_ACTION_H_
#define ADD_SLIDE_ACTION_H_

#include "IAction.h"
#include <string>

namespace ppt_cli {

class AddSlideAction : public IAction {
public:
    explicit AddSlideAction(const std::string& title = "Untitled");
    
    void execute(Presentation& presentation) override;
    void undo(Presentation& presentation) override;
    std::string getDescription() const override;
    
    int getAddedSlideId() const { return addedSlideId_; }

private:
    std::string title_;
    int addedSlideId_ = -1;
};

} // namespace ppt_cli

#endif // ADD_SLIDE_ACTION_H_
