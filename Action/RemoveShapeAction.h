#ifndef REMOVE_SHAPE_ACTION_H_
#define REMOVE_SHAPE_ACTION_H_

#include "IAction.h"
#include "../Document/Elements/Shape.h"
#include <memory>

namespace ppt_cli {

class RemoveShapeAction : public IAction {
public:
    explicit RemoveShapeAction(int shapeId);
    
    void execute(Presentation& presentation) override;
    void undo(Presentation& presentation) override;
    std::string getDescription() const override;

private:
    int shapeId_;
    std::unique_ptr<IShape> removedShape_;
    size_t slideIndex_ = 0;
};

} // namespace ppt_cli

#endif // REMOVE_SHAPE_ACTION_H_
