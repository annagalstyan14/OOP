#ifndef FETCH_H_
#define FETCH_H_

#include <iostream>
#include <string>
#include "Tokenizer.h"

class Fetch {
public:
    std::string getCommand(std::istream& in) {
        std::string line;
        std::cout << "(ppt) : ";
        if (!std::getline(in, line)) {
            return "";
        }
        return line;
    }
};

#endif // !FETCH_H_