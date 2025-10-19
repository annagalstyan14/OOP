#include "Tokenizer.h"
#include <cctype>
#include <map>

namespace ppt_cli {

namespace {
const std::map<std::string, Keyword> keyword_map = {
    {"add", Keyword::ADD},
    {"remove", Keyword::REMOVE},
    {"edit", Keyword::EDIT},
    {"set", Keyword::SET},
    {"at", Keyword::AT},
    {"slide", Keyword::SLIDE},
    {"title", Keyword::TITLE},
    {"bullet", Keyword::BULLET},
    {"shape", Keyword::SHAPE}
};
}

Tokenizer::Tokenizer(const std::string& input)
    : stream_(nullptr), input_string_(input), string_stream_(std::make_unique<std::istringstream>(input)),
      buffer_pos_(0), buffer_size_(0), current_token_(TokenType::END) {
    stream_ = string_stream_.get();
    advance();
}

Tokenizer::Tokenizer(std::istream& input)
    : stream_(&input), string_stream_(nullptr), buffer_pos_(0), buffer_size_(0), current_token_(TokenType::END) {
    std::string line;
    if (!std::getline(input, line)) {
        input_string_ = "";
    } else {
        input_string_ = line;
    }
    string_stream_ = std::make_unique<std::istringstream>(input_string_);
    stream_ = string_stream_.get();
    advance();
}

Tokenizer::Tokenizer()
    : stream_(&std::cin), string_stream_(nullptr), buffer_pos_(0), buffer_size_(0), current_token_(TokenType::END) {
    std::string line;
    if (!std::getline(std::cin, line)) {
        if (std::cin.eof() || std::cin.fail()) {
            input_string_ = "";
            std::cin.clear();
        }
    } else {
        input_string_ = line;
    }
    string_stream_ = std::make_unique<std::istringstream>(input_string_);
    stream_ = string_stream_.get();
    advance();
}

Token Tokenizer::peek() const {
    return current_token_;
}

Token Tokenizer::next() {
    Token result = current_token_;
    advance();
    return result;
}

bool Tokenizer::isEnd() const {
    return current_token_.getType() == TokenType::END;
}

char Tokenizer::peekChar() {
    if (buffer_pos_ >= buffer_size_ && !fillBuffer()) {
        return '\0';
    }
    return buffer_[buffer_pos_];
}

char Tokenizer::getChar() {
    if (buffer_pos_ >= buffer_size_ && !fillBuffer()) {
        return '\0';
    }
    return buffer_[buffer_pos_++];
}

Keyword Tokenizer::lookupKeyword(const std::string& word) const {
    auto it = keyword_map.find(word);
    return it != keyword_map.end() ? it->second : Keyword::UNKNOWN;
}

std::string Tokenizer::readWord() {
    std::string word;
    char c = peekChar();
    while (std::isalpha(c) || c == '-') {
        word += getChar();
        c = peekChar();
    }
    return word;
}

std::string Tokenizer::readNumber() {
    std::string number;
    char c = peekChar();
    while (std::isdigit(c) || c == '.') {
        number += getChar();
        c = peekChar();
    }
    return number;
}

std::string Tokenizer::readString() {
    std::string str;
    char quote = getChar();
    char c = peekChar();
    while (c != quote && c != '\0') {
        str += getChar();
        c = peekChar();
    }
    if (c == quote) {
        getChar();
    }
    return str;
}

void Tokenizer::skipWhitespace() {
    char c = peekChar();
    while (std::isspace(c)) {
        getChar();
        c = peekChar();
    }
}

void Tokenizer::advance() {
    skipWhitespace();
    char c = peekChar();
    if (c == '\0') {
        current_token_ = Token(TokenType::END);
        return;
    }

    if (c == '=' || c == ';' || c == '{' || c == '}' || c == '(' || c == ')') {
        std::string symbol(1, getChar());
        current_token_ = Token(TokenType::SYMBOL, symbol);
        return;
    }

    if (c == '"' || c == '\'') {
        std::string str = readString();
        current_token_ = Token(TokenType::STRING, str);
        return;
    }

    if (std::isdigit(c)) {
        std::string number = readNumber();
        current_token_ = Token(TokenType::NUMBER, number);
        return;
    }

    if (std::isalpha(c) || c == '-') {
        std::string word = readWord();
        Keyword keyword = lookupKeyword(word);
        if (keyword != Keyword::UNKNOWN) {
            current_token_ = Token(TokenType::KEYWORD, word, keyword);
        } else {
            current_token_ = Token(TokenType::UNKNOWN, word);
        }
        return;
    }

    std::string unknown(1, getChar());
    current_token_ = Token(TokenType::UNKNOWN, unknown);
}

bool Tokenizer::fillBuffer() {
    if (!stream_ || stream_->eof() || stream_->fail()) {
        buffer_size_ = 0;
        buffer_pos_ = 0;
        return false;
    }
    stream_->read(buffer_, kBufferSize);
    buffer_size_ = stream_->gcount();
    buffer_pos_ = 0;
    return buffer_size_ > 0;
}

void Tokenizer::checkLength() const {
    if (buffer_pos_ > buffer_size_) {
        std::cerr << "Warning: Buffer position exceeds size\n";
    }
}

}