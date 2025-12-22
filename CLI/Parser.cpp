#include "Parser.h"
#include <cctype>
#include <algorithm>
#include <unordered_map>

namespace ppt_cli {

const Parser::ParserState Parser::transitionTable[][21] = {
    /* START */  { ParserState::ERROR, ParserState::ACTION, ParserState::ACTION,
                   ParserState::ACTION, ParserState::ACTION, ParserState::ACTION,
                   ParserState::ACTION, ParserState::ACTION, ParserState::ACTION, 
                   ParserState::ACTION, ParserState::ACTION, ParserState::ACTION,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::DONE },

    /* ACTION */ { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::DONE, ParserState::DONE, ParserState::DONE, 
                   ParserState::DONE, ParserState::DONE, ParserState::DONE,
                   ParserState::TARGET, ParserState::TARGET, ParserState::TARGET,
                   ParserState::TARGET, ParserState::TARGET, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ARG, ParserState::DONE },

    /* TARGET */ { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR, 
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ARG, ParserState::ARG, ParserState::ARG,
                   ParserState::ARG, ParserState::ARG, ParserState::ARG,
                   ParserState::ARG, ParserState::ARG, ParserState::DONE },

    /* ARG */    { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR, 
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ARG, ParserState::ARG, ParserState::ARG,
                   ParserState::ARG, ParserState::ARG, ParserState::ARG,
                   ParserState::ARG, ParserState::ARG, ParserState::DONE },

    /* DONE */   { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR, 
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::DONE },

    /* ERROR */  { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR, 
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                   ParserState::ERROR, ParserState::ERROR, ParserState::ERROR }
};

Parser::Parser(std::istream& stream) 
    : tokenizer_(std::make_unique<Tokenizer>(stream)) {}

std::string Parser::toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return out;
}

Parser::TokenClass Parser::classify(const Token& token) {
    if (token.getType() == TokenType::END) return TokenClass::END;
    if (token.getType() == TokenType::NUMBER) return TokenClass::NUMBER;
    if (token.getType() == TokenType::STRING) return TokenClass::STRING;
    if (token.getType() != TokenType::KEYWORD) return TokenClass::UNKNOWN;

    static const std::unordered_map<std::string, TokenClass> map = {
        {"add", TokenClass::ADD}, {"remove", TokenClass::REMOVE},
        {"list", TokenClass::LIST}, {"edit", TokenClass::EDIT}, {"set", TokenClass::SET},
        {"save", TokenClass::SAVE}, {"load", TokenClass::LOAD},
        {"new", TokenClass::NEW}, {"open", TokenClass::OPEN},
        {"export", TokenClass::EXPORT}, {"render", TokenClass::RENDER},
        {"slide", TokenClass::SLIDE}, {"text", TokenClass::TEXT},
        {"title", TokenClass::TITLE}, {"bullet", TokenClass::BULLET},
        {"shape", TokenClass::SHAPE}, {"at", TokenClass::AT}
    };
    auto it = map.find(toLower(token.getValue()));
    return it != map.end() ? it->second : TokenClass::UNKNOWN;
}

const std::string& Parser::getError() const {
    return errorMsg_;
}

ParsedCommand Parser::parse() {
    errorMsg_.clear();
    ParsedCommand cmd;
    std::vector<std::string> rawArgs;
    ParserState state = ParserState::START;

    while (true) {
        const Token tok = tokenizer_->getToken();
        TokenClass cls = classify(tok);
        int sIdx = static_cast<int>(state);
        int tIdx = static_cast<int>(cls);
        ParserState next = transitionTable[sIdx][tIdx];

        if (next == ParserState::ERROR) {
            errorMsg_ = "Unexpected '" + tok.getValue() + "' after " +
                       (state == ParserState::START ? "nothing" :
                        state == ParserState::ACTION ? "command" :
                        state == ParserState::TARGET ? "object" : "argument");
            return cmd;
        }

        state = next;

        switch (state) {
            case ParserState::ACTION:
                if (cls >= TokenClass::ADD && cls <= TokenClass::RENDER)
                    cmd.action = toLower(tok.getValue());
                break;
            case ParserState::TARGET:
                if (cls >= TokenClass::SLIDE && cls <= TokenClass::SHAPE)
                    cmd.target = toLower(tok.getValue());
                break;
            case ParserState::ARG:
                if (cls == TokenClass::AT) {
                    const Token numTok = tokenizer_->getToken();
                    if (classify(numTok) != TokenClass::NUMBER) {
                        errorMsg_ = "Expected number after 'at'";
                        return cmd;
                    }
                    rawArgs.push_back(numTok.getValue());
                } else if (cls == TokenClass::NUMBER || cls == TokenClass::STRING) {
                    rawArgs.push_back(tok.getValue());
                }
                break;
            default:
                break;
        }

        if (cls == TokenClass::END) break;
    }

    // Convert raw args to typed arguments
    for (const auto& s : rawArgs) {
        try {
            double num = std::stod(s);
            cmd.args.push_back(std::make_unique<Argument>(num));
        } catch (...) {
            cmd.args.push_back(std::make_unique<Argument>(s));
        }
    }

    return cmd;
}

} // namespace ppt_cli
