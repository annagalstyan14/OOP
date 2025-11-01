#pragma once
#include "../Tokenizer/Tokenizer.h"
#include "../Command/ICommand.h"
#include "CommandFactory.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

namespace ppt_cli {

class Parser {
public:
    explicit Parser(std::istream& stream);
    std::unique_ptr<ICommand> parse();
    const std::string& getError() const { return errorMsg; }

private:
    enum class ParserState { START, ACTION, TARGET, ARG, DONE, ERROR, COUNT };

    enum class TokenClass {
        UNKNOWN, ADD, REMOVE, LIST, EDIT, SET,
        SLIDE, TEXT, TITLE, BULLET, SHAPE, AT,
        NUMBER, STRING, END
    };

    static TokenClass classify(const Token& token);
    static std::string toLower(const std::string& s);

    Tokenizer tz;
    std::string errorMsg;

    // 6 states × 15 token classes
    static constexpr size_t TOKEN_CLASS_COUNT = 15;
    static const ParserState transitionTable[static_cast<int>(ParserState::COUNT)][TOKEN_CLASS_COUNT];
};

} // namespace ppt_cli