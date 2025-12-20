#ifndef IDESERIALIZER_H_
#define IDESERIALIZER_H_

#include "../Document/Presentation_cli.h"
#include <string>

namespace ppt_cli {

class IDeserializer {
public:
    virtual ~IDeserializer() = default;
    virtual bool deserialize(Presentation& presentation, const std::string& filename) = 0;
    virtual void printToConsole(const std::string& filename) = 0;
};

} // namespace ppt_cli

#endif // IDESERIALIZER_H_
