// Command/LoadCommand.h
#pragma once
#include "ICommand.h"
#include "Argument.h"
#include "../Document/Presentation.h"
#include <vector>
#include <memory>
#include <iostream>

namespace ppt_cli {

class LoadCommand : public ICommand {
public:
    explicit LoadCommand(std::vector<ArgPtr>&& args) : args_(std::move(args)) {}
    
    void execute(Presentation& pres) override {
        std::string filename = "presentation.json";
        
        if (!args_.empty() && args_[0]->isString()) {
            filename = args_[0]->asString();
        }
        
        if (pres.loadFromFile(filename)) {
            std::cout << "Loaded from " << filename << "\n";
        } else {
            std::cout << "Failed to load (file not found or invalid)\n";
        }
    }
    
private:
    std::vector<ArgPtr> args_;
};

} // namespace ppt_cli