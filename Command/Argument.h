#ifndef ARGUMENT_H_
#define ARGUMENT_H_

#include <string>
#include <variant>
#include <memory>
#include <stdexcept>

namespace ppt_cli {

class Argument {
public:
    using Value = std::variant<std::string, double>;
    explicit Argument(std::string s) : value_(std::move(s)) {}
    explicit Argument(double d)       : value_(d) {}

    bool isString() const { return std::holds_alternative<std::string>(value_); }
    bool isNumber() const { return std::holds_alternative<double>(value_); }

    const std::string& asString() const {
        if (!isString()) throw std::runtime_error("Not a string");
        return std::get<std::string>(value_);
    }
    double asNumber() const {
        if (!isNumber()) throw std::runtime_error("Not a number");
        return std::get<double>(value_);
    }

private:
    Value value_;
};

using ArgPtr = std::unique_ptr<Argument>;

} // namespace ppt_cli

#endif