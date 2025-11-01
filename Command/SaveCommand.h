// Command/SaveCommand.h
#pragma once
#include "ICommand.h"
#include "Argument.h"
#include "../Document/Presentation.h"
#include <vector>
#include <memory>
#include <iostream>

namespace ppt_cli {

class SaveCommand : public ICommand {
public:
    explicit SaveCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}
    
    void execute(Presentation& pres) override {
        std::string filename = "presentation.json";
        
        if (!args_.empty() && args_[0]->isString()) {
            filename = args_[0]->asString();
        }
        
        if (pres.saveToFile(filename)) {
            std::cout << "Saved to " << filename << "\n";
        } else {
            std::cout << "Failed to save\n";
        }
    }
    
private:
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli