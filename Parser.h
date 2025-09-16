#pragma once
#include "Tokenizer.h"
#include <string>
#include <stdexcept>
#include <vector>

enum class commandType {
	FILE,
	SLIDE,
	CONTENT,
	FORMAT,
	UNKNOWN
};

struct command {
	commandType type = commandType::UNKNOWN;
	std::string action;
	std::vector<std::string> args;
};

class Parser {
public:
	static command parse(const std::vector<Token>& tokens);
private:
	static commandType classify(const std::string& keyword);
	static std::vector<std::string> extractArgs(const std::vector<Token>& tokens, size_t startIndex);
};