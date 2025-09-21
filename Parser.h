#pragma once
#include "Tokenizer.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <map>

struct ParsedCommand {

	std::string action;
	std::vector<std::string> positionals;
	std::map<std::string, std::string> options;
};

struct ParseError : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class Parser {
public:
	static ParsedCommand parse(std::istringstream& lineStream);
};
