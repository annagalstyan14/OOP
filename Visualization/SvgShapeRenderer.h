#ifndef SVG_SHAPE_RENDERER_H_
#define SVG_SHAPE_RENDERER_H_

#include "IShapeRenderer.h"

namespace ppt_cli {

class SvgShapeRenderer : public IShapeRenderer {
public:
    std::string render(const IShape& shape) const override;

private:
    std::string renderRectangle(const IShape& shape) const;
    std::string renderEllipse(const IShape& shape) const;
    std::string renderTriangle(const IShape& shape) const;
    std::string renderLine(const IShape& shape) const;
};

} // namespace ppt_cli

#endif // SVG_SHAPE_RENDERER_H_
