#include "Tokenizer.h"
#include <vector>
#include <string>
#include <algorithm>

std::vector<Token> Tokenizer::tokenize(const std::string& input) {
	std::vector<Token> tokens;
	std::string current;
	bool inQuotes = false;

	auto isSpace = [](char c) {
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
		};

	auto isDigit = [](char c) {
		return c >= '0' && c <= '9';
		};

	for (size_t i = 0; i< input.size(); ++i) {
		char c = input[i];

		if (c == '"') {
			inQuotes = !inQuotes;
			continue;
		}

		if (!inQuotes && isSpace(c)) {
			if (!current.empty()) {
				if (current.rfind("--", 0) == 0) {
					tokens.push_back({ TokenType::OPTION, current.substr(2) });
				}
				else if (std::all_of(current.begin(), current.end(), isDigit)) {
					tokens.push_back({ TokenType::NUMBER, current });
				}
				else {
					tokens.push_back({ TokenType::COMMAND, current });
				}
				current.clear();
			}
		}
		else {
			current.push_back(c);
		}
	}
	if (!current.empty()) {
		if (current.rfind("--", 0) == 0) {
			tokens.push_back({ TokenType::OPTION, current.substr(2) });
		}
		else if (std::all_of(current.begin(), current.end(), isDigit)) {
			tokens.push_back({ TokenType::NUMBER, current });
		} 
		else {
			tokens.push_back({ TokenType::COMMAND, current });
		}
	}
	tokens.push_back({ TokenType::END, "" });
	return tokens;
}