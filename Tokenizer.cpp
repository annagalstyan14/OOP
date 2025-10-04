#include "Tokenizer.h"
#include <cctype>

void Tokenizer::skipSpace() {
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) 
        ++i;
}

void Tokenizer::advance() {
    skipSpace();
    if (i >= s.size()) {
        current = Token(TokenType::END, "");
        return;
    }

    char c = s[i];
    if (c == '"' || c == '\'') {
        char quote = c; 
        ++i;
        std::string out;
        while (i < s.size() && s[i] != quote) {
            if (s[i] == '\\' && i + 1 < s.size()) {
                out += s[i + 1];
                i += 2;
            } else {
                out += s[i++];
            }
        }
        if (i < s.size() && s[i] == quote) ++i;
        current = Token(TokenType::STRING, out);
        return;
    }

    if (c == '-' && i + 1 < s.size() && s[i + 1] == '-') {
        size_t j = i + 2; // Skip --
        std::string out = "--";
        if (j < s.size() && (std::isalpha(static_cast<unsigned char>(s[j]) || s[j] == '-'))) {
            while (j < s.size() && (std::isalnum(static_cast<unsigned char>(s[j])) || s[j] == '-' || s[j] == '.' || s[j] == '_')) {
                out += s[j++];
            }
            current = Token(TokenType::FLAG, out);
            i = j;
            return;
        }
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
        std::string out;
        size_t j = i;
        while (j < s.size() && (std::isdigit(static_cast<unsigned char>(s[j])) || s[j] == ',')) {
            out += s[j++];
        }
        current = Token(TokenType::NUMBER, out);
        i = j;
        return;
    }

    {
        std::string out;
        size_t j = i;
        while (j < s.size() && !std::isspace(static_cast<unsigned char>(s[j])) && s[j] != '"' && s[j] != '\'') {
            out += s[j++];
        }
        current = Token(TokenType::WORD, out);
        i = j;
        return;
    }
}