#include "Controller.h"
#include <sstream>
#include <algorithm>
#include <fstream>

namespace ppt_cli {

Controller::Controller()
    : editor_(presentation_) {}

void Controller::start() {
    std::cout << "PPT CLI - Presentation Manager\n";
    std::cout << "Type 'help' for a list of commands.\n";
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
           word == "print" || word == "help" ||
           word == "?" || word == "h";
}

void Controller::handleSpecialCommands(const std::string& line, const std::string& firstWord) {
    // Help commands
    if (firstWord == "help" || firstWord == "?" || firstWord == "h") {
        size_t pos = line.find(' ');
        if (pos != std::string::npos) {
            std::string topic = line.substr(pos + 1);
            topic.erase(0, topic.find_first_not_of(" \t"));
            topic.erase(topic.find_last_not_of(" \t") + 1);
            std::transform(topic.begin(), topic.end(), topic.begin(), ::tolower);
            if (!topic.empty()) {
                Help::showCommand(topic);
                return;
            }
        }
        Help::showAll();
        return;
    }
    
    if (firstWord == "exit" || firstWord == "quit") {
        if (presentationOpen_) {
            std::cout << "Save before exiting? (y/n): ";
            std::string response;
            std::getline(std::cin, response);
            if (!response.empty() && (response[0] == 'y' || response[0] == 'Y')) {
                std::string filename = currentFile_.empty() ? "presentation.json" : currentFile_;
                if (ppt::JSONSerializer::save(presentation_, filename)) {
                    std::cout << "Saved to " << filename << "\n";
                }
            }
        }
        std::cout << "Goodbye!\n";
        running_ = false;
        return;
    }
    
    if (firstWord == "new") {
        presentation_ = ppt::Presentation();
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
        
        if (ppt::JSONSerializer::load(presentation_, filename)) {
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
        
        if (ppt::JSONSerializer::save(presentation_, filename)) {
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
        
        // Export slides to SVG using SVGPainter (supports filled shapes)
        bool success = true;
        for (size_t i = 0; i < presentation_.size(); ++i) {
            const auto* slide = presentation_.getSlideAt(i);
            if (!slide) continue;
            
            std::string filename = baseName + "_" + std::to_string(i + 1) + ".svg";
            
            // Use SVGPainter for proper rendering with fill colors
            ppt::SVGPainter painter(800, 600);
            painter.paintSlide(*slide);
            
            if (!painter.saveToFile(filename)) {
                success = false;
                continue;
            }
        }
        
        if (success) {
            std::cout << "Exported " << presentation_.size() << " slides to SVG.\n";
        } else {
            std::cout << "Warning: Some slides could not be exported.\n";
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
        
        ppt::JSONSerializer::print(filename);
        return;
    }
}

} // namespace ppt_cli