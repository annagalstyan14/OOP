// Parser.cpp
#include "Parser.h"
#include "CommandFactory.h"
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace ppt_cli {

// === TRANSITION TABLE ===
const Parser::ParserState Parser::transitionTable[][13] = {
    // 0:UNK 1:ADD 2:REM 3:EDT 4:SET 5:SLD 6:TTL 7:BLT 8:SHP 9:AT 10:NUM 11:STR 12:END
    /* 0: START */  { ParserState::ERROR, ParserState::ACTION, ParserState::ACTION, ParserState::ACTION, ParserState::ACTION,
                      ParserState::ERROR, ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                      ParserState::ERROR, ParserState::ERROR, ParserState::ERROR, ParserState::DONE },
    /* 1: ACTION */ { ParserState::ERROR, ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::TARGET, ParserState::TARGET, ParserState::TARGET, ParserState::TARGET,
                      ParserState::ERROR, ParserState::ARG,    ParserState::ARG,    ParserState::DONE },
    /* 2: TARGET */ { ParserState::ERROR, ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ARG,   ParserState::ARG,    ParserState::ARG,    ParserState::DONE },
    /* 3: ARG */    { ParserState::ERROR, ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ARG,   ParserState::ARG,    ParserState::ARG,    ParserState::DONE },
    /* 4: DONE */   { ParserState::ERROR, ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ERROR, ParserState::ERROR,  ParserState::ERROR,  ParserState::DONE },
    /* 5: ERROR */  { ParserState::ERROR, ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR,
                      ParserState::ERROR, ParserState::ERROR,  ParserState::ERROR,  ParserState::ERROR }
};

// === CONSTRUCTOR ===
Parser::Parser(std::istream& stream) : tz(stream) {}

// === HELPER: toLower ===
std::string Parser::toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return out;
}

// === HELPER: classify ===
Parser::TokenClass Parser::classify(const Token& token) {
    if (token.getType() == TokenType::END) return TokenClass::END;
    if (token.getType() == TokenType::NUMBER) return TokenClass::NUMBER;
    if (token.getType() == TokenType::STRING) return TokenClass::STRING;
    if (token.getType() != TokenType::KEYWORD) return TokenClass::UNKNOWN;

    static const std::unordered_map<std::string, TokenClass> keywordMap = {
        {"add", TokenClass::ADD}, {"remove", TokenClass::REMOVE},
        {"edit", TokenClass::EDIT}, {"set", TokenClass::SET},
        {"slide", TokenClass::SLIDE}, {"title", TokenClass::TITLE},
        {"bullet", TokenClass::BULLET}, {"shape", TokenClass::SHAPE},
        {"at", TokenClass::AT}
    };

    auto it = keywordMap.find(toLower(token.getValue()));
    return it != keywordMap.end() ? it->second : TokenClass::UNKNOWN;
}

// === MAIN: parse ===
std::unique_ptr<ICommand> Parser::parse() {
    errorMsg.clear();
    std::string action, target;
    std::vector<std::string> args;
    ParserState state = ParserState::START;

    while (true) {
        Token tok;
        try {
            tok = tz.getToken();  // Catch exceptions here
        } catch (const std::exception& e) {
            errorMsg = e.what();  // e.g., "Unclosed string literal"
            return nullptr;
        }

        TokenClass cls = classify(tok);
        int sIdx = static_cast<int>(state);
        int tIdx = static_cast<int>(cls);
        ParserState next = transitionTable[sIdx][tIdx];

        if (next == ParserState::ERROR) {
            errorMsg = "Unexpected '" + tok.getValue() + "' after " +
                       (state == ParserState::START ? "nothing" :
                        state == ParserState::ACTION ? "command" :
                        state == ParserState::TARGET ? "object" : "argument");
            return nullptr;
        }

        state = next;

        // Semantic actions
        switch (state) {
            case ParserState::ACTION:
                if (cls >= TokenClass::ADD && cls <= TokenClass::SET)
                    action = toLower(tok.getValue());
                break;
            case ParserState::TARGET:
                if (cls >= TokenClass::SLIDE && cls <= TokenClass::SHAPE)
                    target = toLower(tok.getValue());
                break;
            case ParserState::ARG:
                if (cls == TokenClass::AT) {
                    Token nextTok;
                    try {
                        nextTok = tz.getToken();
                    } catch (const std::exception& e) {
                        errorMsg = e.what();
                        return nullptr;
                    }
                    if (classify(nextTok) != TokenClass::NUMBER) {
                        errorMsg = "Expected number after 'at'";
                        return nullptr;
                    }
                    args.push_back(nextTok.getValue());
                } else {
                    args.push_back(tok.getValue());
                }
                break;
            case ParserState::DONE:
                break;
            default:
                break;
        }

        if (cls == TokenClass::END) break;
    }

    if (action.empty()) {
        errorMsg = "No command found";
        return nullptr;
    }
    if (target.empty()) {
        errorMsg = "No object specified";
        return nullptr;
    }

    try {
        return CommandFactory::createCommand(action, target, args);
    } catch (const std::exception& e) {
        errorMsg = e.what();
        return nullptr;
    }
}
} // namespace ppt_cli