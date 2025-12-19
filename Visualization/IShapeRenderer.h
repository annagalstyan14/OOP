#ifndef ISHAPE_RENDERER_H_
#define ISHAPE_RENDERER_H_

#include "../Document/Elements/IShape.h"
#include <string>

namespace ppt_cli {

class IShapeRenderer {
public:
    virtual ~IShapeRenderer() = default;
    virtual std::string render(const IShape& shape) const = 0;
};

} // namespace ppt_cli

#endif // ISHAPE_RENDERER_H_
