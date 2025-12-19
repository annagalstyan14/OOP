#include "SaveCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Serialization/JsonSerializer.h"
#include <iostream>

namespace ppt_cli {

void SaveCommand::execute(Presentation& pres, Editor& /*editor*/) {
    std::string filename = "presentation.json";
    
    if (!args_.empty() && args_[0]->isString()) {
        filename = args_[0]->asString();
    }
    
    JsonSerializer serializer;
    if (serializer.serialize(pres, filename)) {
        std::cout << "Saved to " << filename << "\n";
    } else {
        std::cout << "Failed to save\n";
    }
}

} // namespace ppt_cli
