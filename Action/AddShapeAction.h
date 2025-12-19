#ifndef ADD_SHAPE_ACTION_H_
#define ADD_SHAPE_ACTION_H_

#include "IAction.h"
#include "../Document/Elements/IShape.h"
#include <string>

namespace ppt_cli {

class AddShapeAction : public IAction {
public:
    AddShapeAction(const std::string& name, ShapeType type, const std::string& color);
    
    void execute(Presentation& presentation) override;
    void undo(Presentation& presentation) override;
    std::string getDescription() const override;

private:
    std::string name_;
    ShapeType type_;
    std::string color_;
    int addedShapeId_ = -1;
    size_t slideIndex_ = 0;
};

} // namespace ppt_cli

#endif // ADD_SHAPE_ACTION_H_
