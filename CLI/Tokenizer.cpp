#include "Tokenizer.h"
#include <cctype>
#include <iostream>

namespace ppt_cli {

const std::unordered_map<std::string, Keyword> Tokenizer::keywordMap_ = {
    {"add", Keyword::ADD}, {"remove", Keyword::REMOVE},
    {"list", Keyword::LIST}, {"edit", Keyword::EDIT}, {"set", Keyword::SET},
    {"save", Keyword::SAVE}, {"load", Keyword::LOAD},
    {"new", Keyword::NEW}, {"open", Keyword::OPEN},
    {"export", Keyword::EXPORT}, {"render", Keyword::RENDER},
    {"at", Keyword::AT}, {"slide", Keyword::SLIDE},
    {"text", Keyword::TEXT}, {"title", Keyword::TITLE},
    {"bullet", Keyword::BULLET}, {"shape", Keyword::SHAPE}
};

Token::Token(TokenType type, std::string value, Keyword keyword)
    : type_(type), value_(std::move(value)), keyword_(keyword) {}

TokenType Token::getType() const noexcept { return type_; }
const std::string& Token::getValue() const noexcept { return value_; }
Keyword Token::getKeyword() const noexcept { return keyword_; }

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
        auto it = keywordMap_.find(word);
        Keyword kw = it != keywordMap_.end() ? it->second : Keyword::UNKNOWN;
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
    pos_++;
    return str;
}

Keyword Tokenizer::lookupKeyword(const std::string& word) const {
    auto it = keywordMap_.find(word);
    return it != keywordMap_.end() ? it->second : Keyword::UNKNOWN;
}

} // namespace ppt_cli
