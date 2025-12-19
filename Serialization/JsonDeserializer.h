#ifndef JSON_DESERIALIZER_H_
#define JSON_DESERIALIZER_H_

#include "IDeserializer.h"

namespace ppt_cli {

class JsonDeserializer : public IDeserializer {
public:
    bool deserialize(Presentation& presentation, const std::string& filename) override;
    void printToConsole(const std::string& filename) override;
};

} // namespace ppt_cli

#endif // JSON_DESERIALIZER_H_
