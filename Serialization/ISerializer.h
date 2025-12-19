#ifndef ISERIALIZER_H_
#define ISERIALIZER_H_

#include "../Document/Presentation.h"
#include <string>

namespace ppt_cli {

class ISerializer {
public:
    virtual ~ISerializer() = default;
    virtual bool serialize(const Presentation& presentation, const std::string& filename) = 0;
};

} // namespace ppt_cli

#endif // ISERIALIZER_H_
