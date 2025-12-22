#ifndef CLI_HELP_H_
#define CLI_HELP_H_

#include <string>
#include <iostream>

namespace ppt_cli {

class Help {
public:
    static void showAll() {
        std::cout << "Usage: <command> [options]\n\n";
        std::cout << "Session:\n";
        std::cout << "  new                     Create new presentation\n";
        std::cout << "  open <file>             Open presentation from file\n";
        std::cout << "  save [file]             Save presentation to file\n";
        std::cout << "  exit                    Exit program\n\n";
        
        std::cout << "Slides:\n";
        std::cout << "  add slide [title]       Add new slide\n";
        std::cout << "  remove slide <id>       Remove slide by id\n\n";
        
        std::cout << "Shapes:\n";
        std::cout << "  add shape <type> [fill] [stroke] [x] [y] [w] [h]\n";
        std::cout << "                          Add shape (rectangle|circle|line)\n";
        std::cout << "  remove shape <id>       Remove shape by id\n\n";
        
        std::cout << "Text:\n";
        std::cout << "  add text <n> \"content\"\n";
        std::cout << "                          Add text object\n";
        std::cout << "  remove text <id>        Remove text by id\n\n";
        
        std::cout << "Modify:\n";
        std::cout << "  set shape <id> fill <color>      Set fill color\n";
        std::cout << "  set shape <id> stroke <color>    Set stroke color\n";
        std::cout << "  set shape <id> pos <x> <y>       Set position\n";
        std::cout << "  set shape <id> size <w> <h>      Set size\n\n";
        
        std::cout << "History:\n";
        std::cout << "  undo                    Undo last action\n";
        std::cout << "  redo                    Redo last undone action\n\n";
        
        std::cout << "View:\n";
        std::cout << "  list                    Show all slides\n";
        std::cout << "  list slides             List slide summary\n";
        std::cout << "  list current            Show current slide\n";
        std::cout << "  list shapes             List shapes on current slide\n\n";
        
        std::cout << "Export:\n";
        std::cout << "  export [name]           Export slides to SVG\n";
        std::cout << "  print [file]            Print JSON contents\n\n";
        
        std::cout << "Type 'help <command>' for more information.\n";
    }
    
    static void showCommand(const std::string& command) {
        if (command == "new") {
            std::cout << "Usage: new\n";
            std::cout << "Create a new empty presentation.\n";
        } 
        else if (command == "open") {
            std::cout << "Usage: open <filename>\n";
            std::cout << "Open presentation from JSON file.\n";
        } 
        else if (command == "save") {
            std::cout << "Usage: save [filename]\n";
            std::cout << "Save presentation to JSON file.\n";
            std::cout << "Default: presentation.json\n";
        } 
        else if (command == "load") {
            std::cout << "Usage: load [filename]\n";
            std::cout << "Load presentation from JSON file.\n";
        } 
        else if (command == "add") {
            std::cout << "Usage:\n";
            std::cout << "  add slide [title]\n";
            std::cout << "  add shape <type> [fill] [stroke] [x] [y] [w] [h]\n";
            std::cout << "  add text <n> \"content\"\n\n";
            std::cout << "Shape types: rectangle, circle, line\n";
            std::cout << "Colors: CSS names (red, blue) or 'none' for transparent\n\n";
            std::cout << "Examples:\n";
            std::cout << "  add slide \"Title\"\n";
            std::cout << "  add shape \"rectangle\" \"blue\" \"black\" 100 100 200 150\n";
            std::cout << "  add shape \"circle\" \"red\" \"black\" 300 200 100 100\n";
        } 
        else if (command == "remove") {
            std::cout << "Usage:\n";
            std::cout << "  remove slide <id>\n";
            std::cout << "  remove shape <id>\n";
            std::cout << "  remove text <id>\n";
        } 
        else if (command == "list") {
            std::cout << "Usage:\n";
            std::cout << "  list            Show entire presentation\n";
            std::cout << "  list slides     Show slide summary\n";
            std::cout << "  list current    Show current slide details\n";
            std::cout << "  list shapes     List objects on current slide\n";
        } 
        else if (command == "set") {
            std::cout << "Usage: set shape <id> <property> <value>\n\n";
            std::cout << "Properties:\n";
            std::cout << "  fill <color>         Set fill color\n";
            std::cout << "  stroke <color>       Set stroke color\n";
            std::cout << "  pos <x> <y>          Set position\n";
            std::cout << "  size <w> <h>         Set size\n";
            std::cout << "  geom <x> <y> <w> <h> Set full geometry\n\n";
            std::cout << "Examples:\n";
            std::cout << "  set shape 1 fill red\n";
            std::cout << "  set shape 1 pos 100 200\n";
            std::cout << "  set shape 1 size 150 100\n";
        } 
        else if (command == "undo") {
            std::cout << "Usage: undo\n";
            std::cout << "Undo the last action.\n";
        } 
        else if (command == "redo") {
            std::cout << "Usage: redo\n";
            std::cout << "Redo the last undone action.\n";
        } 
        else if (command == "export") {
            std::cout << "Usage: export [basename]\n";
            std::cout << "Export all slides to SVG files.\n";
            std::cout << "Output: <basename>_1.svg, <basename>_2.svg, ...\n";
            std::cout << "Default basename: slide\n";
        } 
        else if (command == "print") {
            std::cout << "Usage: print [filename]\n";
            std::cout << "Print raw JSON contents of a file.\n";
        } 
        else if (command == "exit" || command == "quit") {
            std::cout << "Usage: exit\n";
            std::cout << "Exit the program. Prompts to save if needed.\n";
        } 
        else {
            std::cout << "Unknown command: " << command << "\n";
            std::cout << "Type 'help' for available commands.\n";
        }
    }
};

} // namespace ppt_cli

#endif // CLI_HELP_H_