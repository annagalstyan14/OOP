#include "Controller.h"
#include <algorithm>
#include <sstream>

namespace ppt {

Controller::Controller() : editor_(presentation_) {}

void Controller::start() {
    std::cout << "PPT CLI - Presentation Editor\n";
    std::cout << "Commands: new, open <file>, save [file], add, remove, list, undo, redo, export, exit\n";
    std::cout << "Type 'new' to create a presentation or 'open <file>' to load one.\n\n";
    
    while (running_) {
        std::cout << "ppt> ";
        std::cout.flush();
        
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        run(line);
    }
}

void Controller::run(const std::string& line) {
    // Trim whitespace
    std::string trimmed = line;
    size_t start = trimmed.find_first_not_of(" \t");
    if (start == std::string::npos) return;
    trimmed = trimmed.substr(start);
    size_t end = trimmed.find_last_not_of(" \t");
    trimmed = trimmed.substr(0, end + 1);
    
    if (trimmed.empty()) return;
    
    std::string cmd = getFirstWord(trimmed);
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    
    if (isSpecialCommand(cmd)) {
        handleSpecialCommand(trimmed, cmd);
        return;
    }
    
    if (!presentationOpen_) {
        std::cout << "No presentation open. Use 'new' or 'open <file>' first.\n";
        return;
    }
    
    Parser parser(trimmed, factory_);
    auto command = parser.parse();
    
    if (!command) {
        std::cout << "Error: " << parser.getError() << "\n";
        return;
    }
    
    try {
        command->execute(presentation_, editor_);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

std::string Controller::getFirstWord(const std::string& line) const {
    size_t pos = line.find(' ');
    return pos != std::string::npos ? line.substr(0, pos) : line;
}

bool Controller::isSpecialCommand(const std::string& cmd) const {
    return cmd == "exit" || cmd == "quit" || cmd == "new" || 
           cmd == "open" || cmd == "save" || cmd == "undo" ||
           cmd == "redo" || cmd == "export" || cmd == "print";
}

void Controller::handleSpecialCommand(const std::string& line, const std::string& cmd) {
    if (cmd == "exit" || cmd == "quit") {
        if (presentationOpen_) {
            std::cout << "Save before exiting? (y/n): ";
            std::string response;
            std::getline(std::cin, response);
            if (!response.empty() && (response[0] == 'y' || response[0] == 'Y')) {
                std::string filename = currentFile_.empty() ? "presentation.json" : currentFile_;
                if (JSONSerializer::save(presentation_, filename)) {
                    std::cout << "Saved to " << filename << "\n";
                }
            }
        }
        std::cout << "Goodbye!\n";
        running_ = false;
        return;
    }
    
    if (cmd == "new") {
        presentation_.clear();
        editor_.clearHistory();
        presentationOpen_ = true;
        currentFile_ = "";
        std::cout << "Created new presentation.\n";
        return;
    }
    
    if (cmd == "open") {
        size_t pos = line.find(' ');
        if (pos == std::string::npos) {
            std::cout << "Usage: open <filename>\n";
            return;
        }
        std::string filename = line.substr(pos + 1);
        // Trim
        size_t start = filename.find_first_not_of(" \t");
        if (start != std::string::npos) {
            filename = filename.substr(start);
        }
        
        if (JSONSerializer::load(presentation_, filename)) {
            editor_.clearHistory();
            presentationOpen_ = true;
            currentFile_ = filename;
            std::cout << "Loaded from " << filename << " (" << presentation_.size() << " slides)\n";
        } else {
            std::cout << "Failed to load: " << filename << "\n";
        }
        return;
    }
    
    if (cmd == "save") {
        if (!presentationOpen_) {
            std::cout << "No presentation open.\n";
            return;
        }
        
        std::string filename = currentFile_.empty() ? "presentation.json" : currentFile_;
        size_t pos = line.find(' ');
        if (pos != std::string::npos) {
            filename = line.substr(pos + 1);
            size_t start = filename.find_first_not_of(" \t");
            if (start != std::string::npos) {
                filename = filename.substr(start);
            }
        }
        
        if (JSONSerializer::save(presentation_, filename)) {
            currentFile_ = filename;
            std::cout << "Saved to " << filename << "\n";
        } else {
            std::cout << "Failed to save.\n";
        }
        return;
    }
    
    if (cmd == "undo") {
        if (!presentationOpen_) {
            std::cout << "No presentation open.\n";
            return;
        }
        editor_.undo();
        return;
    }
    
    if (cmd == "redo") {
        if (!presentationOpen_) {
            std::cout << "No presentation open.\n";
            return;
        }
        editor_.redo();
        return;
    }
    
    if (cmd == "export") {
        if (!presentationOpen_) {
            std::cout << "No presentation open.\n";
            return;
        }
        
        std::string baseName = "slide";
        size_t pos = line.find(' ');
        if (pos != std::string::npos) {
            baseName = line.substr(pos + 1);
            size_t start = baseName.find_first_not_of(" \t");
            if (start != std::string::npos) {
                baseName = baseName.substr(start);
            }
        }
        
        if (RenderCommand::exportPresentation(presentation_, baseName)) {
            std::cout << "Exported " << presentation_.size() << " slides.\n";
        }
        return;
    }
    
    if (cmd == "print") {
        std::string filename = "presentation.json";
        size_t pos = line.find(' ');
        if (pos != std::string::npos) {
            filename = line.substr(pos + 1);
            size_t start = filename.find_first_not_of(" \t");
            if (start != std::string::npos) {
                filename = filename.substr(start);
            }
        }
        JSONSerializer::print(filename);
        return;
    }
}

} // namespace ppt
