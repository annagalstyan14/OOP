#ifndef JSON_SERIALIZER_CLI_H_
#define JSON_SERIALIZER_CLI_H_

#include "ISerializer.h"

namespace ppt_cli {

class JsonSerializer : public ISerializer {
public:
    bool serialize(const Presentation& presentation, const std::string& filename) override;
};

} // namespace ppt_cli

#endif // JSON_SERIALIZER_CLI_H_
