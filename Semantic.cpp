#include "Semantic.h"
#include <cctype>

SemanticResult SemanticAnalyzer::analyze(const CommandNode& node) {
    if (node.cmd == "create" || node.cmd == "open") {
        if (node.positionArgs.empty() && node.args.empty()) {
            return { false, node.cmd + " requires a filename" };
        }
        return { true, "" };
    } else if (node.cmd == "save") {
        return { true, "" }; // Filename optional
    } else if (node.cmd == "export") {
        if (node.positionArgs.empty() && node.args.empty()) {
            return { false, "export requires a filename" };
        }
        if (node.flags.count("--format") && 
            node.flags.at("--format") != "pdf" && 
            node.flags.at("--format") != "png" && 
            node.flags.at("--format") != "pptx") {
            return { false, "invalid --format value; supported: pdf, png, pptx" };
        }
        if (node.flags.count("--slide") && !validatePositions(node.flags.at("--slide"))) {
            return { false, "invalid --slide value" };
        }
        return { true, "" };
    } else if (node.cmd == "slide") {
        if (node.object == "add") {
            if (node.flags.count("--title") && node.flags.at("--title").empty()) {
                return { false, "--title requires a value" };
            }
            return { true, "" };
        } else if (node.object == "delete") {
            if (!node.flags.count("--index")) {
                return { false, "slide delete requires --index" };
            }
            if (!validatePositions(node.flags.at("--index"))) {
                return { false, "invalid --index value" };
            }
            return { true, "" };
        } else if (node.object == "list") {
            return { true, "" };
        } else if (node.object == "reorder") {
            if (!node.flags.count("--from") || !node.flags.count("--to")) {
                return { false, "slide reorder requires --from and --to" };
            }
            if (!validatePositions(node.flags.at("--from")) || 
                !validatePositions(node.flags.at("--to"))) {
                return { false, "invalid --from or --to value" };
            }
            return { true, "" };
        }
        return { false, "supported: slide add/delete/list/reorder" };
    } else if (node.cmd == "add") {
        if (node.object == "text") {
            if (!node.flags.count("--slide")) {
                return { false, "add text requires --slide" };
            }
            if (!validatePositions(node.flags.at("--slide"))) {
                return { false, "invalid --slide value" };
            }
            if (!node.flags.count("--content")) {
                return { false, "add text requires --content" };
            }
            return { true, "" };
        } else if (node.object == "image") {
            if (!node.flags.count("--slide") || !node.flags.count("--path")) {
                return { false, "add image requires --slide and --path" };
            }
            if (!validatePositions(node.flags.at("--slide"))) {
                return { false, "invalid --slide value" };
            }
            if (node.flags.count("--position") && 
                !validatePositions(node.flags.at("--position"))) {
                return { false, "invalid --position value" };
            }
            return { true, "" };
        } else if (node.object == "chart" || node.object == "table") {
            if (!node.flags.count("--slide") || !node.flags.count("--data")) {
                return { false, "add " + node.object + " requires --slide and --data" };
            }
            if (!validatePositions(node.flags.at("--slide"))) {
                return { false, "invalid --slide value" };
            }
            if (node.object == "chart" && !node.flags.count("--type")) {
                return { false, "add chart requires --type" };
            }
            if (node.object == "chart" && node.flags.count("--type") && 
                node.flags.at("--type") != "bar" && 
                node.flags.at("--type") != "pie" && 
                node.flags.at("--type") != "line") {
                return { false, "invalid --type value; supported: bar, pie, line" };
            }
            return { true, "" };
        }
        return { false, "supported: add text/image/chart/table" };
    } else if (node.cmd == "theme") {
        if (node.object == "apply" && !node.flags.count("--name")) {
            return { false, "theme apply requires --name" };
        }
        return { true, "" };
    } else if (node.cmd == "font") {
        if (node.object == "set") {
            if (!node.flags.count("--slide") || !node.flags.count("--font")) {
                return { false, "font set requires --slide and --font" };
            }
            if (!validatePositions(node.flags.at("--slide"))) {
                return { false, "invalid --slide value" };
            }
            if (node.flags.count("--size") && !validatePositions(node.flags.at("--size"))) {
                return { false, "invalid --size value" };
            }
            return { true, "" };
        }
        return { false, "supported: font set" };
    } else if (node.cmd == "background") {
        if (node.object == "set") {
            if (!node.flags.count("--slide") || !node.flags.count("--color")) {
                return { false, "background set requires --slide and --color" };
            }
            if (!validatePositions(node.flags.at("--slide"))) {
                return { false, "invalid --slide value" };
            }
            return { true, "" };
        }
        return { false, "supported: background set" };
    } else if (node.cmd == "align") {
        if (node.object == "text") {
            if (!node.flags.count("--slide") || !node.flags.count("--position")) {
                return { false, "align text requires --slide and --position" };
            }
            if (!validatePositions(node.flags.at("--slide"))) {
                return { false, "invalid --slide value" };
            }
            return { true, "" };
        }
        return { false, "supported: align text" };
    }
    return { false, "Unknown command: " + node.cmd };
}

bool SemanticAnalyzer::validatePositions(const std::string& s) {
    std::string token;
    for (size_t i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == ',') {
            if (token.empty()) return false;
            if (!isNumber(token)) return false;
            token.clear();
        } else if (!std::isspace(static_cast<unsigned char>(s[i]))) {
            token.push_back(s[i]);
        }
    }
    return true;
}

std::vector<int> SemanticAnalyzer::parsePosition(const std::string& s) {
    std::vector<int> out;
    std::string token;
    for (size_t i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == ',') {
            if (!token.empty()) {
                out.push_back(std::stoi(token));
                token.clear();
            }
        } else if (!std::isspace(static_cast<unsigned char>(s[i]))) {
            token.push_back(s[i]);
        }
    }
    return out;
}

bool SemanticAnalyzer::isNumber(const std::string& t) {
    if (t.empty()) return false;
    for (char c : t) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}