#include "Tokenizer.h"
#include <iostream>
#include <algorithm>

static constexpr size_t MAX_TOKEN_LENGTH = 20;

static inline bool isDelimiter(char c) {
	return c == ' ' || c == '\t' || c == ',';
}

Token nextToken(std::istringstream& iss) {
	char c;
	while (iss.get(c)) {
		if (!isDelimiter(c)) {
			iss.unget();
			break;
		}
	}
	if (!iss.good() || iss.peek() == EOF) {
		return { TokenType::EOC,"" };
	}

	if (iss.peek() == '"') {
		iss.get();
		std::string accum;
		while (iss.get(c)) {
			if (c == '"') break;
			accum.push_back(c);
			if (accum.size() > MAX_TOKEN_LENGTH) {
				throw std::runtime_error("Token exceedes max length");
			}
		}
		return { TokenType::STRING, accum };
	}

	std::string token;
	while (iss.get(c)) {
		if (isDelimiter(c)) break;
		token.push_back(c);
		if (token.size() > MAX_TOKEN_LENGTH) {
			throw std::runtime_error("Token exceeds max length");
		}
	}

	if (token.empty()) {
		return { TokenType::EOC, "" };
	}

	if (token.size() >= 1 && token[0] == '-') {
		size_t index = 0;
		while (index < token.size() && token[index] == '-') ++index;
		std::string name = token.substr(index);
		if (name.empty()) throw std::runtime_error("Invalid option token: '" + token + "'");
		return { TokenType::OPTION, name };
	}

	bool allDigits = !token.empty();
	for (char ch : token) { if (ch < '0' || ch > '9') { allDigits = false; break; } }
	if (allDigits) return { TokenType::NUMBER, token };
	return { TokenType::IDENT, token };
}
