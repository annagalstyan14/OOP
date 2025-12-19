#ifndef REMOVE_SLIDE_ACTION_H_
#define REMOVE_SLIDE_ACTION_H_

#include "IAction.h"
#include "../Document/Slide.h"
#include <optional>

namespace ppt_cli {

class RemoveSlideAction : public IAction {
public:
    explicit RemoveSlideAction(int slideId);
    
    void execute(Presentation& presentation) override;
    void undo(Presentation& presentation) override;
    std::string getDescription() const override;

private:
    int slideId_;
    std::optional<Slide> removedSlide_;
    size_t removedIndex_ = 0;
};

} // namespace ppt_cli

#endif // REMOVE_SLIDE_ACTION_H_
