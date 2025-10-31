#pragma once
#include "Token.h"
#include <istream>
#include <memory>
#include <string>
#include <sstream>

namespace ppt_cli {

class Tokenizer {
public:
    explicit Tokenizer(std::istream& input);

    Token getToken();  // Replaces peek() + next()
    bool eof() const;

private:
    Keyword lookupKeyword(const std::string& word) const;
    std::string readWord();
    std::string readNumber();
    std::string readString();
    void skipWhitespace();
    char peekChar();
    char getChar();
    bool fillBuffer();

    std::istream* stream_;
    std::unique_ptr<std::istringstream> owned_stream_;
    std::string buffer_;
    size_t pos_ = 0;
};

} // namespace ppt_cli