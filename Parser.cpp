#include "Parser.h"
#include <iostream>
#include <algorithm>

extern Token nextToken(std::istringstream& iss); 

enum class State {
    START, FILE_CMD, EXPECT_EXPORT_OPTS, SLIDE, CONTENT_ADD, FORMAT, READ_OPTIONS, ERROR, DONE
};

static bool matches(const Token& t, const char* s) {
    return t.type == TokenType::IDENT && t.value == s;
}

ParsedCommand Parser::parse(std::istringstream& lineStream) {
    ParsedCommand out;
    State state = State::START;

    while (true) {
        Token tk = nextToken(lineStream);

        if (tk.type == TokenType::EOC) {
            if (state == State::START) {
                throw ParseError("Empty command");
            }
            state = State::DONE;
            break;
        }

        switch (state) {
        case State::START:
            if (tk.type != TokenType::IDENT) throw ParseError("Expected command keyword at start");
            if (tk.value == "create" || tk.value == "open" || tk.value == "save") {
                out.action = tk.value;
                state = State::READ_OPTIONS;
            }
            else if (tk.value == "export") {
                out.action = tk.value;
                state = State::EXPECT_EXPORT_OPTS;
            }
            else if (tk.value == "slide") {

                Token sub = nextToken(lineStream);
                if (sub.type != TokenType::IDENT) throw ParseError("Expected slide sub-command after 'slide'");
                if (sub.value == "add" || sub.value == "delete" || sub.value == "list" || sub.value == "reorder") {
                    out.action = std::string("slide ") + sub.value;
                    if (sub.value == "list") {
                        state = State::READ_OPTIONS;
                    }
                    else {
                        state = State::READ_OPTIONS;
                    }
                }
                else {
                    throw ParseError("Invalid slide sub-command: " + sub.value);
                }
            }
            else if (tk.value == "add") {
                Token sub = nextToken(lineStream);
                if (sub.type != TokenType::IDENT) throw ParseError("Expected add sub-command (text/image/chart/table)");
                if (sub.value == "text" || sub.value == "image" || sub.value == "chart" || sub.value == "table") {
                    out.action = std::string("add ") + sub.value;
                    state = State::READ_OPTIONS;
                }
                else {
                    throw ParseError("Invalid add sub-command: " + sub.value);
                }
            }
            else if (tk.value == "theme" || tk.value == "font" || tk.value == "background" || tk.value == "align") {

                Token sub = nextToken(lineStream);
                if (sub.type != TokenType::IDENT) throw ParseError("Expected formatting sub-command");
                out.action = tk.value + std::string(" ") + sub.value;
                state = State::READ_OPTIONS;
            }
            else {
                throw ParseError("Unknown top-level command: " + tk.value);
            }
            break;

        case State::EXPECT_EXPORT_OPTS:

            if (tk.type == TokenType::IDENT || tk.type == TokenType::STRING) {
                out.positionals.push_back(tk.value);
                state = State::READ_OPTIONS;
            }
            else {
                throw ParseError("Expected filename after export");
            }
            break;

        case State::READ_OPTIONS:
            if (tk.type == TokenType::OPTION) {
                std::string optName = tk.value;
                Token possibleValue = nextToken(lineStream);
                if (possibleValue.type == TokenType::IDENT || possibleValue.type == TokenType::STRING || possibleValue.type == TokenType::NUMBER) {
                    out.options[optName] = possibleValue.value;
                }
                else if (possibleValue.type == TokenType::OPTION) {
                    out.options[optName] = "";

                    tk = possibleValue;
                    out.options[possibleValue.value] = "";
                }
                else if (possibleValue.type == TokenType::EOC) {
                    out.options[optName] = "";
                }
                else {
                    throw ParseError("Unexpected token after option --" + optName);
                }
            }
            else if (tk.type == TokenType::IDENT || tk.type == TokenType::STRING || tk.type == TokenType::NUMBER) {
                out.positionals.push_back(tk.value);
            }
            else {
                throw ParseError("Unexpected token in options/args");
            }
            break;

        default:
            throw ParseError("Parser reached invalid state");
        }
    }

    if (out.action == "create" || out.action == "open") {
        if (out.positionals.empty()) throw ParseError(out.action + " requires a filename");
    }

    if (out.action.rfind("slide ", 0) == 0) {

    }

    return out;
}
