#include "Parser.h"
#include <stdexcept>

namespace ppt_cli {

Parser::Parser(std::istream& stream)
    : tokenizer_(stream), current_token_(tokenizer_.peek()) {
    initializeTransitionTable();
}

std::unique_ptr<ICommand> Parser::parse() {
    CommandType action = CommandType::Unknown;
    ObjectType target = ObjectType::Unknown;
    std::vector<std::string> args;
    current_state_ = State::START;

    while (current_state_ != State::DONE && current_state_ != State::ERROR) {
        processToken(current_token_, action, target, args);
        current_token_ = nextToken();
    }

    if (current_state_ == State::ERROR) {
        throw std::runtime_error("Syntax error: Invalid command sequence");
    }

    if (action == CommandType::Add && target == ObjectType::Slide) {
        return std::make_unique<AddSlide>(args);
    } else if (action == CommandType::Add && target == ObjectType::Shape) {
        if (args.empty()) {
            throw std::runtime_error("Syntax error: Shape requires a type argument");
        }
        return std::make_unique<AddShape>(args[0]);
    } else if (action == CommandType::Add && target == ObjectType::Bullet) {
        if (args.empty()) {
            throw std::runtime_error("Syntax error: Bullet requires a text argument");
        }
        return std::make_unique<AddBullet>(args[0]);
    } else if (action == CommandType::Add && target == ObjectType::Title) {
        if (args.empty()) {
            throw std::runtime_error("Syntax error: Title requires a text argument");
        }
        return std::make_unique<AddTitle>(args[0]);
    } else if (action == CommandType::Set && target == ObjectType::Title) {
        if (args.empty()) {
            throw std::runtime_error("Syntax error: Title requires a text argument");
        }
        return std::make_unique<SetTitle>(args[0]);
    } else if (action == CommandType::Remove && target == ObjectType::Slide) {
        return std::make_unique<RemoveSlide>(args);
    } else if (action == CommandType::Edit && target == ObjectType::Slide) {
        if (args.empty()) {
            throw std::runtime_error("Syntax error: Edit requires a slide number");
        }
        return std::make_unique<EditSlide>(args[0]);
    }

    throw std::runtime_error("Syntax error: Invalid command or target");
}

void Parser::processToken(Token token, CommandType& action, ObjectType& target, std::vector<std::string>& args) {
    std::string token_value = token.getType() == TokenType::KEYWORD ? token.getValue() : token.getValue();
    size_t state_idx = static_cast<size_t>(current_state_);
    size_t token_idx = 0;

    if (token.getType() == TokenType::KEYWORD) {
        switch (token.getKeyword()) {
            case Keyword::ADD: token_idx = 1; break;
            case Keyword::REMOVE: token_idx = 2; break;
            case Keyword::EDIT: token_idx = 3; break;
            case Keyword::SET: token_idx = 4; break;
            case Keyword::SLIDE: token_idx = 5; break;
            case Keyword::TITLE: token_idx = 6; break;
            case Keyword::BULLET: token_idx = 7; break;
            case Keyword::SHAPE: token_idx = 8; break;
            case Keyword::AT: token_idx = 9; break;
            default: token_idx = 0; break;
        }
    } else if (token.getType() == TokenType::NUMBER) {
        token_idx = 10;
    } else if (token.getType() == TokenType::STRING) {
        token_idx = 11;
    } else if (token.getType() == TokenType::END) {
        token_idx = 12;
    } else {
        token_idx = 0;
    }

    State next_state = transition_table_[state_idx][token_idx];
    if (next_state == State::ERROR) {
        current_state_ = State::ERROR;
        return;
    }

    switch (current_state_) {
        case State::START:
            if (token.getType() == TokenType::KEYWORD) {
                switch (token.getKeyword()) {
                    case Keyword::ADD: action = CommandType::Add; break;
                    case Keyword::REMOVE: action = CommandType::Remove; break;
                    case Keyword::EDIT: action = CommandType::Edit; break;
                    case Keyword::SET: action = CommandType::Set; break;
                    default: current_state_ = State::ERROR; return;
                }
            }
            break;
        case State::ACTION:
            if (token.getType() == TokenType::KEYWORD) {
                switch (token.getKeyword()) {
                    case Keyword::SLIDE: target = ObjectType::Slide; break;
                    case Keyword::TITLE: target = ObjectType::Title; break;
                    case Keyword::BULLET: target = ObjectType::Bullet; break;
                    case Keyword::SHAPE: target = ObjectType::Shape; break;
                    default: current_state_ = State::ERROR; return;
                }
            }
            break;
        case State::TARGET:
        case State::ARGUMENTS:
            if (token.getType() == TokenType::NUMBER || token.getType() == TokenType::STRING) {
                args.push_back(token.getValue());
            } else if (token.getType() == TokenType::KEYWORD && token.getKeyword() == Keyword::AT) {
                current_token_ = nextToken();
                if (current_token_.getType() == TokenType::NUMBER) {
                    args.push_back(current_token_.getValue());
                } else {
                    current_state_ = State::ERROR;
                    return;
                }
            }
            break;
        default:
            break;
    }

    current_state_ = next_state;
}

Token Parser::nextToken() {
    return tokenizer_.next();
}

void Parser::initializeTransitionTable() {
    for (size_t i = 0; i < kTableSize; ++i) {
        for (size_t j = 0; j < kTableSize; ++j) {
            transition_table_[i][j] = State::ERROR;
        }
    }

    transition_table_[static_cast<size_t>(State::START)][1] = State::ACTION; // add
    transition_table_[static_cast<size_t>(State::START)][2] = State::ACTION; // remove
    transition_table_[static_cast<size_t>(State::START)][3] = State::ACTION; // edit
    transition_table_[static_cast<size_t>(State::START)][4] = State::ACTION; // set

    transition_table_[static_cast<size_t>(State::ACTION)][5] = State::TARGET; // slide
    transition_table_[static_cast<size_t>(State::ACTION)][6] = State::TARGET; // title
    transition_table_[static_cast<size_t>(State::ACTION)][7] = State::TARGET; // bullet
    transition_table_[static_cast<size_t>(State::ACTION)][8] = State::TARGET; // shape

    transition_table_[static_cast<size_t>(State::TARGET)][9] = State::ARGUMENTS; // at
    transition_table_[static_cast<size_t>(State::TARGET)][10] = State::ARGUMENTS; // number
    transition_table_[static_cast<size_t>(State::TARGET)][11] = State::ARGUMENTS; // string
    transition_table_[static_cast<size_t>(State::TARGET)][12] = State::DONE; // end

    transition_table_[static_cast<size_t>(State::ARGUMENTS)][9] = State::ARGUMENTS; // at
    transition_table_[static_cast<size_t>(State::ARGUMENTS)][10] = State::ARGUMENTS; // number
    transition_table_[static_cast<size_t>(State::ARGUMENTS)][11] = State::ARGUMENTS; // string
    transition_table_[static_cast<size_t>(State::ARGUMENTS)][12] = State::DONE; // end
}

}