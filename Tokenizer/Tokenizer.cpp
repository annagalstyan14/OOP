#include "Tokenizer.h"
#include <cctype>
#include <map>
#include <sstream>
#include <iostream>
namespace ppt_cli {

namespace {
const std::map<std::string, Keyword> kKeywordMap = {
    {"add", Keyword::ADD}, {"remove", Keyword::REMOVE},
    {"edit", Keyword::EDIT}, {"set", Keyword::SET},
    {"at", Keyword::AT}, {"slide", Keyword::SLIDE},
    {"title", Keyword::TITLE}, {"bullet", Keyword::BULLET},
    {"shape", Keyword::SHAPE}
};
}

Tokenizer::Tokenizer(std::istream& input) {
    if (&input == &std::cin) {
        std::string line;
        if (std::getline(input, line)) {
            owned_stream_ = std::make_unique<std::istringstream>(line);
            stream_ = owned_stream_.get();
            buffer_ = line;
        } else {
            stream_ = nullptr;
        }
    } else {
        stream_ = &input;
        std::ostringstream oss;
        oss << input.rdbuf();
        buffer_ = oss.str();
    }
    pos_ = 0;
    skipWhitespace();
}

Token Tokenizer::getToken() {
    skipWhitespace();
    if (pos_ >= buffer_.size()) return Token(TokenType::END);

    char c = buffer_[pos_];

    if (c == '"' || c == '\'') {
        return Token(TokenType::STRING, readString());
    }
    if (std::isdigit(c)) {
        return Token(TokenType::NUMBER, readNumber());
    }
    if (std::isalpha(c) || c == '-') {
        std::string word = readWord();
        auto it = kKeywordMap.find(word);
        Keyword kw = it != kKeywordMap.end() ? it->second : Keyword::UNKNOWN;
        TokenType type = (kw != Keyword::UNKNOWN) ? TokenType::KEYWORD : TokenType::UNKNOWN;
        return Token(type, word, kw);
    }
    if (c == '=' || c == ';' || c == '{' || c == '}' || c == '(' || c == ')') {
        pos_++;
        return Token(TokenType::SYMBOL, std::string(1, c));
    }

    pos_++;
    return Token(TokenType::UNKNOWN, std::string(1, c));
}

bool Tokenizer::eof() const {
    return pos_ >= buffer_.size();
}

char Tokenizer::peekChar() {
    return (pos_ < buffer_.size()) ? buffer_[pos_] : '\0';
}

char Tokenizer::getChar() {
    return (pos_ < buffer_.size()) ? buffer_[pos_++] : '\0';
}

void Tokenizer::skipWhitespace() {
    while (pos_ < buffer_.size() && std::isspace(buffer_[pos_])) pos_++;
}

std::string Tokenizer::readWord() {
    size_t start = pos_;
    while (pos_ < buffer_.size() && (std::isalpha(buffer_[pos_]) || buffer_[pos_] == '-')) pos_++;
    return buffer_.substr(start, pos_ - start);
}

std::string Tokenizer::readNumber() {
    size_t start = pos_;
    while (pos_ < buffer_.size() && (std::isdigit(buffer_[pos_]) || buffer_[pos_] == '.')) pos_++;
    return buffer_.substr(start, pos_ - start);
}

std::string Tokenizer::readString() {
    char quote = getChar();
    if (quote != '"' && quote != '\'') return "";

    size_t start = pos_;
    while (pos_ < buffer_.size() && buffer_[pos_] != quote) pos_++;
    
    if (pos_ >= buffer_.size()) {
        throw std::runtime_error("Unclosed string literal");
    }

    std::string str = buffer_.substr(start, pos_ - start);
    pos_++;  // skip closing quote
    return str;
}

Keyword Tokenizer::lookupKeyword(const std::string& word) const {
    auto it = kKeywordMap.find(word);
    return it != kKeywordMap.end() ? it->second : Keyword::UNKNOWN;
}

} // namespace ppt_cli