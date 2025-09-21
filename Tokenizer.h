#pragma once
#include <string>
#include <sstream>

enum class TokenType {
	IDENT, OPTION, STRING, NUMBER, EOC, INVALID
};

struct Token {
	TokenType type;
	std::string value;
};

//class Tokenizer {
//public:
	//static std::vector<Token> tokenize(const std::string& input);
//};
