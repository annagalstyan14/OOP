#ifndef PARSER_H_
#define PARSER_H_

#include "Tokenizer.h"
#include "../Command/ICommand.h"
#include "../Repository/CommandRepository.h"
#include <memory>
#include <string>
#include <vector>

namespace ppt_cli {

class Parser {
public:
    Parser(std::istream& stream, CommandRepository& repo);
    
    std::unique_ptr<ICommand> parse();
    const std::string& getError() const { return errorMsg_; }

private:
    enum class ParserState { START, ACTION, TARGET, ARG, DONE, ERROR, COUNT };
    enum class TokenClass {
        UNKNOWN,
        ADD, REMOVE, LIST, EDIT, SET, SAVE, LOAD, NEW, OPEN,
        SLIDE, TEXT, TITLE, BULLET, SHAPE, AT,
        NUMBER, STRING, END
    };

    static TokenClass classify(const Token& token);
    static std::string toLower(const std::string& s);

    std::unique_ptr<Tokenizer> tokenizer_;
    CommandRepository& commandRepo_;
    std::string errorMsg_;

    static const ParserState transitionTable[static_cast<int>(ParserState::COUNT)][19];
};

} // namespace ppt_cli

#endif // PARSER_H_
