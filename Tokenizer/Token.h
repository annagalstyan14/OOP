#pragma once
#include <string>

namespace ppt_cli {

enum class TokenType {
    KEYWORD,
    NUMBER,
    STRING,
    FLAG,
    SYMBOL,
    END,
    UNKNOWN
};

enum class Keyword {
    ADD, REMOVE, EDIT, SET, AT,
    SLIDE, TITLE, BULLET, SHAPE,
    UNKNOWN
};

class Token {
public:
    Token(TokenType type = TokenType::END, std::string value = "", Keyword keyword = Keyword::UNKNOWN)
        : type_(type), value_(std::move(value)), keyword_(keyword) {}
    
    TokenType getType() const { return type_; }
    std::string getValue() const { return value_; }
    Keyword getKeyword() const { return keyword_; }

private:
    TokenType type_;
    std::string value_;
    Keyword keyword_;
};

}