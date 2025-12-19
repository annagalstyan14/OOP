#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "Token.h"
#include <istream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>

namespace ppt_cli {

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

#endif // TOKENIZER_H_
