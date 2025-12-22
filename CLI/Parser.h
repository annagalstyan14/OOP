#ifndef CLI_PARSER_H_
#define CLI_PARSER_H_

#include "Tokenizer.h"
#include "Argument.h"
#include <memory>
#include <string>
#include <vector>

namespace ppt_cli {

struct ParsedCommand {
    std::string action;
    std::string target;
    std::vector<ArgPtr> args;
    
    bool isValid() const { return !action.empty(); }
};

class Parser {
public:
    explicit Parser(std::istream& stream);
    
    ParsedCommand parse();
    const std::string& getError() const;

private:
    enum class ParserState { START, ACTION, TARGET, ARG, DONE, ERROR, COUNT };
    enum class TokenClass {
        UNKNOWN,
        ADD, REMOVE, LIST, EDIT, SET, SAVE, LOAD, NEW, OPEN, EXPORT, RENDER,
        SLIDE, TEXT, TITLE, BULLET, SHAPE, AT,
        NUMBER, STRING, END
    };

    static TokenClass classify(const Token& token);
    static std::string toLower(const std::string& s);

    std::unique_ptr<Tokenizer> tokenizer_;
    std::string errorMsg_;

    static const ParserState transitionTable[static_cast<int>(ParserState::COUNT)][21];
};

} // namespace ppt_cli

#endif // CLI_PARSER_H_
