#include "Tokenizer.h"
#include <cctype>
#include <algorithm>
#include <stdexcept>

namespace ppt {

const std::unordered_map<std::string, Keyword> Tokenizer::keywordMap_ = {
    {"add", Keyword::ADD}, {"remove", Keyword::REMOVE},
    {"list", Keyword::LIST}, {"edit", Keyword::EDIT}, {"set", Keyword::SET},
    {"save", Keyword::SAVE}, {"load", Keyword::LOAD},
    {"new", Keyword::NEW}, {"open", Keyword::OPEN},
    {"slide", Keyword::SLIDE}, {"text", Keyword::TEXT},
    {"title", Keyword::TITLE}, {"shape", Keyword::SHAPE},
    {"rectangle", Keyword::RECTANGLE}, {"circle", Keyword::CIRCLE},
    {"line", Keyword::LINE}, {"at", Keyword::AT}
};

Tokenizer::Tokenizer(std::istream& input) {
    init(input);
}

Tokenizer::Tokenizer(const std::string& input) {
    ownedStream_ = std::make_unique<std::istringstream>(input);
    init(*ownedStream_);
}

void Tokenizer::init(std::istream& input) {
    std::ostringstream oss;
    oss << input.rdbuf();
    buffer_ = oss.str();
    pos_ = 0;
}

Token Tokenizer::getToken() {
    skipWhitespace();
    
    if (pos_ >= buffer_.size()) {
        return Token(TokenType::END, "");
    }
    
    char c = buffer_[pos_];
    
    // String literal
    if (c == '"' || c == '\'') {
        return Token(TokenType::STRING, readString());
    }
    
    // Number
    if (std::isdigit(c) || (c == '-' && pos_ + 1 < buffer_.size() && std::isdigit(buffer_[pos_ + 1]))) {
        return Token(TokenType::NUMBER, readNumber());
    }
    
    // Word (keyword or identifier)
    if (std::isalpha(c) || c == '_') {
        std::string word = readWord();
        std::string lower = word;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        
        Keyword kw = lookupKeyword(lower);
        TokenType type = (kw != Keyword::UNKNOWN) ? TokenType::KEYWORD : TokenType::UNKNOWN;
        return Token(type, lower, kw);
    }
    
    // Symbol
    if (c == '=' || c == ';' || c == '{' || c == '}' || c == '(' || c == ')' || c == ',') {
        pos_++;
        return Token(TokenType::SYMBOL, std::string(1, c));
    }
    
    // Unknown character
    pos_++;
    return Token(TokenType::UNKNOWN, std::string(1, c));
}

bool Tokenizer::eof() const {
    return pos_ >= buffer_.size();
}

void Tokenizer::skipWhitespace() {
    while (pos_ < buffer_.size() && std::isspace(buffer_[pos_])) {
        pos_++;
    }
}

std::string Tokenizer::readWord() {
    size_t start = pos_;
    while (pos_ < buffer_.size() && (std::isalnum(buffer_[pos_]) || buffer_[pos_] == '_' || buffer_[pos_] == '-')) {
        pos_++;
    }
    return buffer_.substr(start, pos_ - start);
}

std::string Tokenizer::readNumber() {
    size_t start = pos_;
    if (buffer_[pos_] == '-') pos_++;
    
    while (pos_ < buffer_.size() && (std::isdigit(buffer_[pos_]) || buffer_[pos_] == '.')) {
        pos_++;
    }
    return buffer_.substr(start, pos_ - start);
}

std::string Tokenizer::readString() {
    char quote = buffer_[pos_++];
    size_t start = pos_;
    
    while (pos_ < buffer_.size() && buffer_[pos_] != quote) {
        if (buffer_[pos_] == '\\' && pos_ + 1 < buffer_.size()) {
            pos_++; // Skip escape character
        }
        pos_++;
    }
    
    std::string result = buffer_.substr(start, pos_ - start);
    
    if (pos_ < buffer_.size()) {
        pos_++; // Skip closing quote
    }
    
    return result;
}

Keyword Tokenizer::lookupKeyword(const std::string& word) const {
    auto it = keywordMap_.find(word);
    return it != keywordMap_.end() ? it->second : Keyword::UNKNOWN;
}

} // namespace ppt
