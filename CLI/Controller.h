#ifndef CLI_CONTROLLER_H_
#define CLI_CONTROLLER_H_

#include "Parser.h"
#include "CommandFactory.h"
#include "../Document/Presentation.h"
#include "../Action/Editor.h"
#include <string>
#include <memory>
#include <iostream>

namespace ppt_cli {

class Controller {
public:
    Controller();
    
    void run(const std::string& line);
    void start();
    
    ppt::Presentation& getPresentation();
    ppt::Editor& getEditor();
    bool isRunning() const;
    void stop();

private:
    void handleSpecialCommands(const std::string& line, const std::string& firstWord);
    bool isSpecialCommand(const std::string& word) const;
    
    ppt::Presentation presentation_;
    ppt::Editor editor_;
    bool running_ = true;
    bool presentationOpen_ = false;
    std::string currentFile_;
};

} // namespace ppt_cli

#endif // CLI_CONTROLLER_H_
