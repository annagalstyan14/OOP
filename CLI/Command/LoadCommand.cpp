#include "LoadCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Serialization/JsonDeserializer.h"
#include <iostream>

namespace ppt_cli {

void LoadCommand::execute(Presentation& pres, Editor& editor) {
    std::string filename = "presentation.json";
    
    if (!args_.empty() && args_[0]->isString()) {
        filename = args_[0]->asString();
    }
    
    JsonDeserializer deserializer;
    if (deserializer.deserialize(pres, filename)) {
        editor.clearHistory();
        std::cout << "Loaded from " << filename << "\n";
    } else {
        std::cout << "Failed to load (file not found or invalid)\n";
    }
}

} // namespace ppt_cli
