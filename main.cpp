#include "CLI/Controller/Controller.h"
#include <iostream>

int main() {
    try {
        ppt_cli::Controller controller;
        controller.start();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
