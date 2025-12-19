#include "Controller.h"
#include <sstream>
#include <algorithm>

namespace ppt_cli {

Controller::Controller()
    : editor_(presentation_) {}

void Controller::start() {
    std::cout << "PPT CLI - Presentation Manager\n";
    std::cout << "Commands: new, open <file>, save [file], add, remove, list, undo, redo, export, exit\n";
    std::cout << "Start by typing 'new' to create a new presentation or 'open <filename>' to load one.\n\n";
    
    while (running_) {
        std::cout << "ppt-cli> ";
        std::cout.flush();
        
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        run(line);
    }
}

void Controller::run(const std::string& line) {
    if (line.empty()) return;
    
    std::string trimmed = line;
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
    
    if (trimmed.empty()) return;
    
    std::string firstWord = trimmed.substr(0, trimmed.find(' '));
    std::transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::tolower);
    
    if (isSpecialCommand(firstWord)) {
        handleSpecialCommands(trimmed, firstWord);
        return;
    }
    
    if (!presentationOpen_) {
        std::cout << "Error: No presentation open. Use 'new' to create or 'open <file>' to load one.\n";
        return;
    }
    
    std::istringstream iss(trimmed);
    Parser parser(iss, commandRepo_);
    auto cmd = parser.parse();
    
    if (!cmd) {
        std::cout << "Error: " << parser.getError() << "\n";
        return;
    }
    
    try {
        cmd->execute(presentation_, editor_);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

bool Controller::isSpecialCommand(const std::string& word) const {
    return word == "exit" || word == "quit" || 
           word == "new" || word == "open" || 
           word == "save" || word == "undo" || 
           word == "redo" || word == "export" ||
           word == "print";
}

void Controller::handleSpecialCommands(const std::string& line, const std::string& firstWord) {
    if (firstWord == "exit" || firstWord == "quit") {
        if (presentationOpen_) {
            std::cout << "Save before exiting? (y/n): ";
            std::string response;
            std::getline(std::cin, response);
            if (!response.empty() && (response[0] == 'y' || response[0] == 'Y')) {
                JsonSerializer serializer;
                std::string filename = currentFile_.empty() ? "presentation.json" : currentFile_;
                if (serializer.serialize(presentation_, filename)) {
                    std::cout << "Saved to " << filename << "\n";
                }
            }
        }
        std::cout << "Goodbye!\n";
        running_ = false;
        return;
    }
    
    if (firstWord == "new") {
        presentation_ = Presentation();
        editor_.clearHistory();
        presentationOpen_ = true;
        currentFile_ = "";
        std::cout << "Created new presentation.\n";
        return;
    }
    
    if (firstWord == "open") {
        size_t pos = line.find(' ');
        if (pos == std::string::npos) {
            std::cout << "Error: Usage: open <filename>\n";
            return;
        }
        std::string filename = line.substr(pos + 1);
        filename.erase(0, filename.find_first_not_of(" \t"));
        
        JsonDeserializer deserializer;
        if (deserializer.deserialize(presentation_, filename)) {
            editor_.clearHistory();
            presentationOpen_ = true;
            currentFile_ = filename;
            std::cout << "Loaded from " << filename << " (" << presentation_.size() << " slides)\n";
        } else {
            std::cout << "Error: Could not load " << filename << "\n";
        }
        return;
    }
    
    if (firstWord == "save") {
        if (!presentationOpen_) {
            std::cout << "Error: No presentation open.\n";
            return;
        }
        
        size_t pos = line.find(' ');
        std::string filename;
        if (pos != std::string::npos) {
            filename = line.substr(pos + 1);
            filename.erase(0, filename.find_first_not_of(" \t"));
        } else {
            filename = currentFile_.empty() ? "presentation.json" : currentFile_;
        }
        
        JsonSerializer serializer;
        if (serializer.serialize(presentation_, filename)) {
            currentFile_ = filename;
            std::cout << "Saved to " << filename << "\n";
        } else {
            std::cout << "Error: Failed to save to " << filename << "\n";
        }
        return;
    }
    
    if (firstWord == "undo") {
        if (!presentationOpen_) {
            std::cout << "Error: No presentation open.\n";
            return;
        }
        editor_.undo();
        return;
    }
    
    if (firstWord == "redo") {
        if (!presentationOpen_) {
            std::cout << "Error: No presentation open.\n";
            return;
        }
        editor_.redo();
        return;
    }
    
    if (firstWord == "export") {
        if (!presentationOpen_) {
            std::cout << "Error: No presentation open.\n";
            return;
        }
        
        size_t pos = line.find(' ');
        std::string baseName = "slide";
        if (pos != std::string::npos) {
            baseName = line.substr(pos + 1);
            baseName.erase(0, baseName.find_first_not_of(" \t"));
        }
        
        auto shapeRenderer = std::make_unique<SvgShapeRenderer>();
        SlideRenderer renderer(std::move(shapeRenderer));
        
        if (renderer.exportPresentation(presentation_.getSlides(), baseName)) {
            std::cout << "Exported " << presentation_.size() << " slides to SVG.\n";
        } else {
            std::cout << "Error: Failed to export slides.\n";
        }
        return;
    }
    
    if (firstWord == "print") {
        size_t pos = line.find(' ');
        std::string filename = "presentation.json";
        if (pos != std::string::npos) {
            filename = line.substr(pos + 1);
            filename.erase(0, filename.find_first_not_of(" \t"));
        }
        
        JsonDeserializer deserializer;
        deserializer.printToConsole(filename);
        return;
    }
}

} // namespace ppt_cli
