#include "Parser/Parser.h"
#include "Semantic/Semantic.h"
#include "Parser/CommandFactory.h"
#include "Document/Presentation.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    ppt_cli::Presentation pres;
    bool presentationOpen = false;
    std::string currentFile = "";

    std::cout << "PPT CLI - Presentation Manager\n";
    std::cout << "Commands: new, open <file>, save [file], add, remove, list, exit\n";
    std::cout << "Start by typing 'new' to create a new presentation or 'open <filename>' to load one.\n\n";
    
    while (true) {
        std::cout << "ppt-cli> ";
        std::cout.flush();
        
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        if (line.empty()) continue;
        
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        // Convert first word to lowercase for comparison
        std::string firstWord = line.substr(0, line.find(' '));
        std::transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::tolower);
        
        // Handle special commands
        if (firstWord == "exit" || firstWord == "quit") {
            if (presentationOpen) {
                std::cout << "Save before exiting? (y/n): ";
                std::string response;
                std::getline(std::cin, response);
                if (!response.empty() && (response[0] == 'y' || response[0] == 'Y')) {
                    if (!currentFile.empty()) {
                        pres.saveToFile(currentFile);
                        std::cout << "Saved to " << currentFile << "\n";
                    } else {
                        std::cout << "Enter filename to save (or press Enter to skip): ";
                        std::string filename;
                        std::getline(std::cin, filename);
                        if (!filename.empty()) {
                            pres.saveToFile(filename);
                            std::cout << "Saved to " << filename << "\n";
                        }
                    }
                }
            }
            std::cout << "Goodbye!\n";
            break;
        }
        
        if (firstWord == "new") {
            pres = ppt_cli::Presentation();  // Reset to new presentation
            presentationOpen = true;
            currentFile = "";
            std::cout << "Created new presentation.\n";
            continue;
        }
        
        if (firstWord == "open") {
            size_t pos = line.find(' ');
            if (pos == std::string::npos) {
                std::cout << "Error: Usage: open <filename>\n";
                continue;
            }
            std::string filename = line.substr(pos + 1);
            filename.erase(0, filename.find_first_not_of(" \t"));
            
            if (pres.loadFromFile(filename)) {
                presentationOpen = true;
                currentFile = filename;
                std::cout << "Loaded from " << filename << " (" << pres.size() << " slides)\n";
            } else {
                std::cout << "Error: Could not load " << filename << "\n";
            }
            continue;
        }
        
        if (firstWord == "save") {
            if (!presentationOpen) {
                std::cout << "Error: No presentation open. Use 'new' or 'open <file>' first.\n";
                continue;
            }
            
            size_t pos = line.find(' ');
            std::string filename;
            if (pos != std::string::npos) {
                filename = line.substr(pos + 1);
                filename.erase(0, filename.find_first_not_of(" \t"));
            } else {
                filename = currentFile.empty() ? "presentation.json" : currentFile;
            }
            
            if (pres.saveToFile(filename)) {
                currentFile = filename;
                std::cout << "Saved to " << filename << "\n";
            } else {
                std::cout << "Error: Failed to save to " << filename << "\n";
            }
            continue;
        }
        
        // For all other commands, check if presentation is open
        if (!presentationOpen) {
            std::cout << "Error: No presentation open. Use 'new' to create or 'open <file>' to load one.\n";
            continue;
        }
        
        // Parse and execute regular commands
        std::istringstream iss(line);
        ppt_cli::Parser parser(iss);
        auto cmd = parser.parse();
        
        if (!cmd) {
            std::cout << "Error: " << parser.getError() << "\n";
            continue;
        }
        
        try {
            cmd->execute(pres);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}