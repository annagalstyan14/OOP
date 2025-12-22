#include "Argument.h"

namespace ppt_cli {

Argument::Argument(const std::string& s) : value_(s) {}

Argument::Argument(double d) : value_(d) {}

bool Argument::isString() const {
    return std::holds_alternative<std::string>(value_);
}

bool Argument::isNumber() const {
    return std::holds_alternative<double>(value_);
}

const std::string& Argument::asString() const {
    if (!isString()) {
        throw std::runtime_error("Argument is not a string");
    }
    return std::get<std::string>(value_);
}

double Argument::asNumber() const {
    if (!isNumber()) {
        throw std::runtime_error("Argument is not a number");
    }
    return std::get<double>(value_);
}

} // namespace ppt_cli
