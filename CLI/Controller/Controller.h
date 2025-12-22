#ifndef CLI_CONTROLLER_H_
#define CLI_CONTROLLER_H_

#include "../Parser/Parser.h"
#include "../Repository/CommandRepository.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Document/JSONSerializer.h"
#include "../../Rendering/SVGPainter.h"
#include <string>
#include <memory>
#include <iostream>

namespace ppt_cli {

class Controller {
public:
    Controller();
    
    void run(const std::string& line);
    void start();
    
    ppt::Presentation& getPresentation() { return presentation_; }
    ppt::Editor& getEditor() { return editor_; }
    bool isRunning() const { return running_; }
    void stop() { running_ = false; }

private:
    void handleSpecialCommands(const std::string& line, const std::string& firstWord);
    bool isSpecialCommand(const std::string& word) const;
    
    CommandRepository commandRepo_;
    ppt::Presentation presentation_;
    ppt::Editor editor_;
    bool running_ = true;
    bool presentationOpen_ = false;
    std::string currentFile_;
};

} // namespace ppt_cli

#endif // CLI_CONTROLLER_H_
