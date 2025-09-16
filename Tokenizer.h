#pragma once
#include <string>
#include <vector>

enum class TokenType {
	COMMAND, OPTION, STRING, NUMBER, END
};

struct Token {
	TokenType type;
	std::string value;
};

class Tokenizer {
public:
	static std::vector<Token> tokenize(const std::string& input);
};