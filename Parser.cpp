#include "Parser.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

commandType Parser::classify(const std::string& keyword) {
	if (keyword == "create" || keyword == "open" || keyword == "save" || keyword == "export") {
		return commandType::FILE;
	}
	if (keyword == "slide") {
		return commandType::SLIDE;
	}
	if (keyword == "add") {
		return commandType::CONTENT;
	}
	if (keyword == "theme") {
		return commandType::FORMAT;
	}
	return commandType::UNKNOWN;
}

std::vector<std::string> Parser::extractArgs(const std::vector<Token>& tokens, size_t startIndex) {
	std::vector<std::string> args;
	for (size_t i = startIndex; i < tokens.size(); ++i) {
		if (tokens[i].type == TokenType::END) break;
		args.push_back(tokens[i].value);
	}
	return args;
}

command Parser::parse(const std::vector<Token>& tokens) {
	if (tokens.empty()) {
		throw std::runtime_error("Empty input can't be parsed");
	}

	size_t index = 0;
	Token first = tokens[index];
	if (first.type != TokenType::COMMAND) {
		throw std::runtime_error("Command must start with a keyword");
	}

	command cmd;
	cmd.type = classify(first.value);
	cmd.action = first.value;
	index++;

	if (cmd.type == commandType::SLIDE || cmd.type == commandType::CONTENT || cmd.type == commandType::FORMAT) {
		if (index >= tokens.size()) {
			throw std::runtime_error("Missing arguments");
		}

		Token second = tokens[index];
		if (second.type != TokenType::COMMAND) {
			throw std::runtime_error("Expected sub-command after" + first.value);
		}

		cmd.action += " " + second.value;
		index++;
	}

	cmd.args = extractArgs(tokens, index);

	for (const std::string& arg : cmd.args) {
		if (arg.empty()) {
			throw std::runtime_error("Empty argument found");
		}
	}
	return cmd;
}