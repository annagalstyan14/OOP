#include "SaveCommand.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Document/JSONSerializer.h"
#include <iostream>

namespace ppt_cli {

void SaveCommand::execute(Presentation& pres, Editor& /*editor*/) {
    std::string filename = "presentation.json";
    
    if (!args_.empty() && args_[0]->isString()) {
        filename = args_[0]->asString();
    }
    
    if (ppt::JSONSerializer::save(pres, filename)) {
        std::cout << "Saved to " << filename << "\n";
    } else {
        std::cout << "Failed to save\n";
    }
}

} // namespace ppt_cli
