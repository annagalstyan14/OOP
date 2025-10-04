#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "Tokenizer.h"

struct CommandNode {
    std::string cmd;
    std::string object;
    std::vector<std::string> positionArgs;
    std::unordered_map<std::string, std::string> flags;
    std::vector<std::string> args;
};

enum class ParserState {
    START,      // Initial state
    COMMAND,    // Parsing command word
    OBJECT,     // Parsing object name
    FLAG,       // Parsing flag name
    FLAG_VALUE, // Parsing flag value
    ARG,        // Parsing arguments
    ERROR,      // Error state
    END         // End state
};

enum class CharCategory {
    ALPHA,      // Alphabetic characters
    DIGIT,      // Numeric characters
    DASH,       // Dash character (-)
    QUOTE,      // Quote characters (" or ')
    SPACE,      // Whitespace
    OTHER       // Any other character
};

class Parser {
public:
    Parser(Tokenizer& tz) : tz(tz) {}
    bool parseDFA(CommandNode& node, std::string& err);
private:
    Tokenizer& tz;
    static std::string toLower(const std::string& s);
    CharCategory categorizeChar(char c);
    ParserState getNextState(ParserState currentState, CharCategory category);
    void processStateAction(ParserState state, char c, CommandNode& node, 
                           std::string& currentWord, std::string& currentFlag, 
                           std::string& currentValue);
};

#endif // !PARSER_H_