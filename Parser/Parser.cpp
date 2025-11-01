#include "Parser.h"
#include "CommandFactory.h"
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace ppt_cli {


constexpr size_t Parser::TOKEN_CLASS_COUNT;

const Parser::ParserState Parser::transitionTable[][Parser::TOKEN_CLASS_COUNT] = {
    /* START */  { ParserState::ERROR,   ParserState::ACTION, ParserState::ACTION,
                   ParserState::ACTION, ParserState::ACTION, ParserState::ACTION,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::DONE },

    /* ACTION */ { ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
               ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
               ParserState::TARGET,  ParserState::TARGET, ParserState::TARGET,
               ParserState::TARGET,  ParserState::TARGET, ParserState::ERROR,
               ParserState::ARG,     ParserState::ERROR,  ParserState::DONE },

    /* TARGET */ { ParserState::ARG,     ParserState::ERROR,  ParserState::ERROR,
                ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                ParserState::ARG,     ParserState::ARG,    ParserState::ARG,
                ParserState::ARG,     ParserState::ARG,    ParserState::ARG,
                ParserState::ARG,     ParserState::ARG,    ParserState::DONE },

    /* ARG */    { ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ARG,     ParserState::ARG,    ParserState::ARG,
                   ParserState::ARG,     ParserState::ARG,    ParserState::ARG,
                   ParserState::ARG,     ParserState::ARG,    ParserState::DONE },

    /* DONE */   { ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::DONE },

    /* ERROR */  { ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR,
                   ParserState::ERROR,   ParserState::ERROR,  ParserState::ERROR }
};


Parser::Parser(std::istream& stream) : tz(stream) {}

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

std::unique_ptr<ICommand> Parser::parse() {
    errorMsg.clear();
    std::string action, target;
    std::vector<std::string> args;
    ParserState state = ParserState::START;
    bool isList = false;  // ← ADD

    while (true) {
        const Token& tok = tz.getToken();
        TokenClass cls = classify(tok);

        // ← SPECIAL CASE
        ParserState next;
        if (state == ParserState::ACTION && isList && cls == TokenClass::UNKNOWN) {
            next = ParserState::ARG;
        } else {
            int sIdx = static_cast<int>(state);
            int tIdx = static_cast<int>(cls);
            next = transitionTable[sIdx][tIdx];
        }

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
                if (cls >= TokenClass::ADD && cls <= TokenClass::SET) {
                    action = toLower(tok.getValue());
                    isList = (action == "list");  // ← SET FLAG
                }
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
    if (action != "list" && target.empty()) { errorMsg = "No target"; return nullptr; }

    std::vector<ArgPtr> converted;
    if (!target.empty()) {
        converted.push_back(std::make_unique<Argument>(target));
    }
    for (const auto& s : args) {
        try {
            double num = std::stod(s);
            converted.push_back(std::make_unique<Argument>(num));
        } catch (...) {
            converted.push_back(std::make_unique<Argument>(s));
        }
    }

    return CommandFactory::createCommand(action, std::move(converted));
}

} // namespace ppt_cli