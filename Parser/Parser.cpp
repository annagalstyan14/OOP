// Parser/Parser.cpp
#include "Parser.h"
#include "CommandFactory.h"
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace ppt_cli {

// ------------------- TRANSITION TABLE (6 × 13) -------------------
// Columns: 0=UNK,1=ADD,2=REMOVE,3=LIST,4=EDIT,5=SET,
//          6=SLIDE,7=TEXT,8=TITLE,9=BULLET,10=SHAPE,11=AT,12=END
const Parser::ParserState Parser::transitionTable[][13] = {
    /* START */ { ParserState::ERROR, ParserState::ACTION, ParserState::ACTION,
                  ParserState::ACTION, ParserState::ACTION, ParserState::ACTION,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::DONE },

    /* ACTION */ { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::TARGET, ParserState::TARGET, ParserState::TARGET,
                  ParserState::TARGET, ParserState::TARGET, ParserState::ERROR,
                  ParserState::DONE },

    /* TARGET */ { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ARG,
                  ParserState::DONE },

    /* ARG */    { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ARG,
                  ParserState::DONE },

    /* DONE */   { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::DONE },

    /* ERROR */  { ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR, ParserState::ERROR, ParserState::ERROR,
                  ParserState::ERROR }
};

// ------------------- CONSTRUCTOR -------------------
Parser::Parser(std::istream& stream) : tz(stream) {}

// ------------------- HELPERS -------------------
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
        {"slide", TokenClass::SLIDE}, {"text", TokenClass::TEXT},
        {"title", TokenClass::TITLE}, {"bullet", TokenClass::BULLET},
        {"shape", TokenClass::SHAPE}, {"at", TokenClass::AT}
    };
    auto it = map.find(toLower(token.getValue()));
    return it != map.end() ? it->second : TokenClass::UNKNOWN;
}

// ------------------- MAIN PARSE -------------------
std::unique_ptr<ICommand> Parser::parse() {
    errorMsg.clear();
    std::string action, target;
    std::vector<std::string> args;
    ParserState state = ParserState::START;

    while (true) {
        const Token& tok = tz.getToken();
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
                    const Token& numTok = tz.getToken();
                    if (classify(numTok) != TokenClass::NUMBER) {
                        errorMsg = "Expected number after 'at'";
                        return nullptr;
                    }
                    args.push_back(numTok.getValue());
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

    if (action.empty()) { errorMsg = "No command"; return nullptr; }
    if (target.empty()) { errorMsg = "No target"; return nullptr; }

    std::vector<ArgPtr> converted;
    for (const auto& s : args) {
        try {
            double num = std::stod(s);
            converted.push_back(std::make_unique<Argument>(num));
        } catch (...) {
            converted.push_back(std::make_unique<Argument>(s));
        }
    }

    return CommandFactory::createCommand(action, target, std::move(converted));
}

} // namespace ppt_cli