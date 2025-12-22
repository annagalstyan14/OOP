#include "Controller.h"
#include "../Document/JSONSerializer.h"
#include "../Rendering/SVGPainter.h"
#include "../Rendering/RenderCommand.h"
#include <sstream>
#include <algorithm>
#include <fstream>

namespace ppt_cli {

Controller::Controller()
    : editor_(presentation_) {}

ppt::Presentation& Controller::getPresentation() { return presentation_; }
ppt::Editor& Controller::getEditor() { return editor_; }
bool Controller::isRunning() const { return running_; }
void Controller::stop() { running_ = false; }

void Controller::start() {
    std::cout << "PPT CLI - Presentation Manager\n";
    std::cout << "Commands: new, open <file>, save [file], add, remove, list, undo, redo, render, exit\n";
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
    Parser parser(iss);
    ParsedCommand parsed = parser.parse();
    
    if (!parsed.isValid()) {
        std::cout << "Error: " << parser.getError() << "\n";
        return;
    }
    
    auto cmd = CommandFactory::create(parsed);
    if (!cmd) {
        std::cout << "Error: Unknown command '" << parsed.action << "'\n";
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
           word == "render" || word == "print";
}

void Controller::handleSpecialCommands(const std::string& line, const std::string& firstWord) {
    if (firstWord == "exit" || firstWord == "quit") {
        if (presentationOpen_) {
            std::cout << "Save before exiting? (y/n): ";
            std::string response;
            std::getline(std::cin, response);
            if (!response.empty() && (response[0] == 'y' || response[0] == 'Y')) {
                std::string filename = currentFile_.empty() ? "presentation.json" : currentFile_;
                if (ppt::Serializer::saveToFile(presentation_, filename)) {
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
        
        if (ppt::Deserializer::loadFromFile(presentation_, filename)) {
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
        
        if (ppt::Serializer::saveToFile(presentation_, filename)) {
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
    
    if (firstWord == "export" || firstWord == "render") {
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
        
        auto painter = std::make_shared<ppt::SVGPainter>(800, 600);
        ppt::RenderCommand renderCmd(painter, baseName, 
                                      ppt::RenderCommand::RenderTarget::PRESENTATION);
        renderCmd.execute(presentation_);
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
