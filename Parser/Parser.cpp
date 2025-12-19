#include "Parser.h"
#include <algorithm>

namespace ppt {

Parser::Parser(const std::string& input, CommandFactory& factory)
    : tokenizer_(std::make_unique<Tokenizer>(input))
    , factory_(factory) {}

std::unique_ptr<ICommand> Parser::parse() {
    error_.clear();
    
    // Get action (first keyword)
    Token actionToken = tokenizer_->getToken();
    
    if (actionToken.type == TokenType::END) {
        error_ = "Empty command";
        return nullptr;
    }
    
    if (actionToken.type != TokenType::KEYWORD) {
        error_ = "Expected command, got: " + actionToken.value;
        return nullptr;
    }
    
    std::string action = actionToken.value;
    
    if (!factory_.hasCommand(action)) {
        error_ = "Unknown command: " + action;
        return nullptr;
    }
    
    // Get target (optional - slide, shape, text, etc.)
    std::string target;
    Token nextToken = tokenizer_->getToken();
    
    if (nextToken.type == TokenType::KEYWORD) {
        target = nextToken.value;
        nextToken = tokenizer_->getToken();
    }
    
    // Collect arguments
    std::vector<ArgPtr> args;
    
    while (nextToken.type != TokenType::END) {
        if (nextToken.type == TokenType::STRING) {
            args.push_back(std::make_unique<Argument>(nextToken.value));
        }
        else if (nextToken.type == TokenType::NUMBER) {
            try {
                double num = std::stod(nextToken.value);
                args.push_back(std::make_unique<Argument>(num));
            } catch (...) {
                args.push_back(std::make_unique<Argument>(nextToken.value));
            }
        }
        else if (nextToken.type == TokenType::KEYWORD) {
            // Keywords after target become string arguments
            args.push_back(std::make_unique<Argument>(nextToken.value));
        }
        else if (nextToken.type != TokenType::SYMBOL) {
            args.push_back(std::make_unique<Argument>(nextToken.value));
        }
        
        nextToken = tokenizer_->getToken();
    }
    
    try {
        return factory_.create(action, target, std::move(args));
    } catch (const std::exception& e) {
        error_ = e.what();
        return nullptr;
    }
}

} // namespace ppt
