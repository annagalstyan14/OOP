#include "Semantic.h"
#include <stdexcept>

namespace ppt_cli {

SemanticAnalyzer::SemanticAnalyzer(const std::vector<std::unique_ptr<ICommand>>& commands)
    : commands_(commands), slide_count_(0) {}

void SemanticAnalyzer::analyze() {
    for (const auto& command : commands_) {
        analyzeCommand(*command);
    }
}

void SemanticAnalyzer::analyzeCommand(const ICommand& command) {
    CommandType action = command.getCommandType();
    ObjectType target = command.getObjectType();
    const auto& args = command.getArgs();

    if (action == CommandType::Unknown || target == ObjectType::Unknown) {
        semanticError("Invalid command or target");
    }

    if (action == CommandType::Add) {
        if (target == ObjectType::Slide) {
            slide_count_++;
            if (!args.empty()) {
                checkSlideExists(args[0]);
            }
        } else if (target == ObjectType::Bullet || target == ObjectType::Shape || target == ObjectType::Title) {
            if (slide_count_ == 0) {
                semanticError("Cannot add bullet, shape, or title without a slide");
            }
            if (args.empty() || args[0].empty()) {
                semanticError("Bullet, shape, or title requires a non-empty string argument");
            }
        } else {
            semanticError("Invalid target for ADD command");
        }
    } else if (action == CommandType::Remove) {
        if (target != ObjectType::Slide) {
            semanticError("REMOVE command only supports slide");
        }
        if (slide_count_ == 0) {
            semanticError("No slides to remove");
        }
        slide_count_--;
        if (!args.empty()) {
            checkSlideExists(args[0]);
        }
    } else if (action == CommandType::Edit) {
        if (target != ObjectType::Slide) {
            semanticError("EDIT command only supports slide");
        }
        if (args.empty()) {
            semanticError("EDIT command requires a slide number");
        }
        checkSlideExists(args[0]);
    } else if (action == CommandType::Set) {
        if (target != ObjectType::Title) {
            semanticError("SET command only supports title");
        }
        if (slide_count_ == 0) {
            semanticError("Cannot set title without a slide");
        }
        if (args.empty() || args[0].empty()) {
            semanticError("Title requires a non-empty string argument");
        }
    }
}

void SemanticAnalyzer::checkSlideExists(const std::string& number) {
    if (!number.empty()) {
        try {
            size_t slide_num = std::stoul(number);
            if (slide_num == 0 || slide_num > slide_count_) {
                semanticError("Slide number " + number + " does not exist");
            }
        } catch (const std::exception&) {
            semanticError("Invalid slide number format: " + number);
        }
    }
}

void SemanticAnalyzer::semanticError(const std::string& message) {
    throw std::runtime_error("Semantic error: " + message);
}

} // namespace ppt_cli