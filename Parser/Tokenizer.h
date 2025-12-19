#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <istream>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace ppt {

enum class TokenType {
    KEYWORD,
    NUMBER,
    STRING,
    SYMBOL,
    END,
    UNKNOWN
};

enum class Keyword {
    ADD, REMOVE, LIST, EDIT, SET, SAVE, LOAD, NEW, OPEN,
    SLIDE, TEXT, TITLE, SHAPE, RECTANGLE, CIRCLE, LINE,
    AT, UNKNOWN
};

struct Token {
    TokenType type = TokenType::END;
    std::string value;
    Keyword keyword = Keyword::UNKNOWN;
    
    Token() = default;
    Token(TokenType t, const std::string& v, Keyword k = Keyword::UNKNOWN)
        : type(t), value(v), keyword(k) {}
};

class Tokenizer {
public:
    explicit Tokenizer(std::istream& input);
    explicit Tokenizer(const std::string& input);
    
    Token getToken();
    bool eof() const;
    
private:
    void init(std::istream& input);
    void skipWhitespace();
    std::string readWord();
    std::string readNumber();
    std::string readString();
    Keyword lookupKeyword(const std::string& word) const;
    
    std::unique_ptr<std::istringstream> ownedStream_;
    std::string buffer_;
    size_t pos_ = 0;
    
    static const std::unordered_map<std::string, Keyword> keywordMap_;
};

} // namespace ppt

#endif // TOKENIZER_H_
