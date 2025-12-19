#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../Document/Presentation.h"
#include "../Document/JSONSerializer.h"
#include "../Action/Editor.h"
#include "../Command/CommandFactory.h"
#include "../Parser/Parser.h"
#include "../Rendering/RenderCommand.h"
#include <string>
#include <iostream>

namespace ppt {

class Controller {
public:
    Controller();
    
    void start();
    void run(const std::string& line);
    
    bool isRunning() const { return running_; }
    void stop() { running_ = false; }
    
    Presentation& getPresentation() { return presentation_; }
    Editor& getEditor() { return editor_; }
    
private:
    bool isSpecialCommand(const std::string& cmd) const;
    void handleSpecialCommand(const std::string& line, const std::string& cmd);
    std::string getFirstWord(const std::string& line) const;
    
    CommandFactory factory_;
    Presentation presentation_;
    Editor editor_;
    bool running_ = true;
    bool presentationOpen_ = false;
    std::string currentFile_;
};

} // namespace ppt

#endif // CONTROLLER_H_
