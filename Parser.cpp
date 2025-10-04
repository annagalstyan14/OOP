#include "Parser.h"
#include <cctype>

std::string Parser::toLower(const std::string& s) {
    std::string out = s;
    for (char& c : out) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return out;
}

bool Parser::parseDFA(CommandNode& node, std::string& err) {
    node.cmd.clear();
    node.object.clear();
    node.flags.clear();
    node.args.clear();
    node.positionArgs.clear();

    Token t = tz.next();
    if (t.type == TokenType::END) {
        err = "empty input";
        return false;
    }

    // Expect first token to be a command (WORD)
    if (t.type != TokenType::WORD) {
        err = "command must be a word";
        return false;
    }
    node.cmd = toLower(t.text);

    // Check if command expects an object (sub-command)
    bool expectsObject = node.cmd == "slide" || node.cmd == "add" || node.cmd == "theme" ||
                        node.cmd == "font" || node.cmd == "background" || node.cmd == "align";

    // Get next token
    t = tz.next();
    if (expectsObject && t.type == TokenType::WORD) {
        node.object = toLower(t.text);
        t = tz.next();
    } else if (!expectsObject && t.type == TokenType::WORD) {
        node.args.push_back(t.text);
        t = tz.next();
    }

    // Process remaining tokens
    while (t.type != TokenType::END) {
        if (t.type == TokenType::FLAG) {
            std::string flag = t.text;
            t = tz.next();
            std::string value;
            if (t.type == TokenType::WORD || t.type == TokenType::STRING || t.type == TokenType::NUMBER) {
                value = t.text;
                t = tz.next();
            }
            node.flags[flag] = value;
        } else if (t.type == TokenType::NUMBER) {
            node.positionArgs.push_back(t.text);
            t = tz.next();
        } else if (t.type == TokenType::WORD || t.type == TokenType::STRING) {
            node.args.push_back(t.text);
            t = tz.next();
        } else {
            err = "invalid token: " + t.text;
            return false;
        }
    }

    // Validate README commands
    if (node.cmd == "slide") {
        if (node.object != "add" && node.object != "delete" && 
            node.object != "list" && node.object != "reorder") {
            err = "Invalid slide sub-command: " + node.object;
            return false;
        }
    } else if (node.cmd == "add") {
        if (node.object != "text" && node.object != "image" && 
            node.object != "chart" && node.object != "table") {
            err = "Invalid add sub-command: " + node.object;
            return false;
        }
    } else if (node.cmd == "theme") {
        if (node.object != "apply") {
            err = "Invalid theme sub-command: " + node.object;
            return false;
        }
    } else if (node.cmd == "font") {
        if (node.object != "set") {
            err = "Invalid font sub-command: " + node.object;
            return false;
        }
    } else if (node.cmd == "background") {
        if (node.object != "set") {
            err = "Invalid background sub-command: " + node.object;
            return false;
        }
    } else if (node.cmd == "align") {
        if (node.object != "text") {
            err = "Invalid align sub-command: " + node.object;
            return false;
        }
    } else if (node.cmd != "create" && node.cmd != "open" && 
               node.cmd != "save" && node.cmd != "export") {
        err = "Unknown command: " + node.cmd;
        return false;
    }

    return true;
}