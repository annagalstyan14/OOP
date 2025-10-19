#pragma once
#include <iostream>
#include <istream>
#include <sstream>
#include <memory>
#include "Token.h"

namespace ppt_cli {

class Tokenizer {
public:
    explicit Tokenizer(const std::string& input);
    explicit Tokenizer(std::istream& input);
    Tokenizer();

    Token peek() const;
    Token next();
    bool isEnd() const;

    char peekChar();
    char getChar();

private:
    Keyword lookupKeyword(const std::string& word) const;

    std::string readWord();
    std::string readNumber();
    std::string readString();
    void skipWhitespace();
    void advance();
    bool fillBuffer();
    void checkLength() const;

    std::istream* stream_;
    std::unique_ptr<std::istringstream> string_stream_;
    std::string input_string_;
    static constexpr size_t kBufferSize = 1024;
    char buffer_[kBufferSize];
    size_t buffer_pos_;
    size_t buffer_size_;
    Token current_token_;
};

}