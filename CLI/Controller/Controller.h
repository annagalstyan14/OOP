#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../Parser/Parser.h"
#include "../Repository/CommandRepository.h"
#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"
#include "../../Serialization/JsonSerializer.h"
#include "../../Serialization/JsonDeserializer.h"
#include "../../Visualization/SlideRenderer.h"
#include "../../Visualization/SvgShapeRenderer.h"
#include <string>
#include <memory>
#include <iostream>

namespace ppt_cli {

class Controller {
public:
    Controller();
    
    void run(const std::string& line);
    void start();
    
    Presentation& getPresentation() { return presentation_; }
    Editor& getEditor() { return editor_; }
    bool isRunning() const { return running_; }
    void stop() { running_ = false; }

private:
    void handleSpecialCommands(const std::string& line, const std::string& firstWord);
    bool isSpecialCommand(const std::string& word) const;
    
    CommandRepository commandRepo_;
    Presentation presentation_;
    Editor editor_;
    bool running_ = true;
    bool presentationOpen_ = false;
    std::string currentFile_;
};

} // namespace ppt_cli

#endif // CONTROLLER_H_
