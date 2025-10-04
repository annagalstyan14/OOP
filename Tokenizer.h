#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <iostream>

enum class TokenType { 
    WORD, 
    FLAG, 
    NUMBER, 
    STRING, 
    END
};

struct Token {
    TokenType type;
    std::string text;
    Token(TokenType t = TokenType::END, std::string s = "") : type(t), text(std::move(s)) {}
};

class Tokenizer {
public:
    Tokenizer(const std::string& line) : s(line), i(0) { advance(); }
    Token peek() const { return current; }
    Token next() {
        Token temp = current;
        advance();
        return temp;
    }
    bool eof() const { return current.type == TokenType::END; }
private:
    std::string s;
    size_t i;
    Token current;
    void skipSpace();
    void advance();
};

#endif // !TOKENIZER_H_