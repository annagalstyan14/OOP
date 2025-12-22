#ifndef CLI_TOKENIZER_H_
#define CLI_TOKENIZER_H_

#include <string>
#include <istream>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace ppt_cli {

enum class TokenType {
    KEYWORD,
    NUMBER,
    STRING,
    SYMBOL,
    END,
    UNKNOWN
};

enum class Keyword {
    ADD, REMOVE, LIST, EDIT, SET, SAVE, LOAD, NEW, OPEN, EXPORT, RENDER,
    AT, SLIDE, TEXT, TITLE, BULLET, SHAPE,
    UNKNOWN
};

class Token {
public:
    Token(TokenType type = TokenType::END, std::string value = "", 
          Keyword keyword = Keyword::UNKNOWN);
    
    TokenType getType() const noexcept;
    const std::string& getValue() const noexcept;
    Keyword getKeyword() const noexcept;

private:
    TokenType type_;
    std::string value_;
    Keyword keyword_;
};

class Tokenizer {
public:
    explicit Tokenizer(std::istream& input);
    
    Token getToken();
    bool eof() const;

private:
    Keyword lookupKeyword(const std::string& word) const;
    std::string readWord();
    std::string readNumber();
    std::string readString();
    void skipWhitespace();
    char peekChar();
    char getChar();

    std::istream* stream_;
    std::unique_ptr<std::istringstream> owned_stream_;
    std::string buffer_;
    size_t pos_ = 0;
    
    static const std::unordered_map<std::string, Keyword> keywordMap_;
};

} // namespace ppt_cli

#endif // CLI_TOKENIZER_H_
