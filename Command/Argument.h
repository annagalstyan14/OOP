// Command/Argument.h
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
        if (!isString())
            throw std::runtime_error("Argument is not a string");
        return std::get<std::string>(value_);
    }

    double asNumber() const {
        if (!isNumber())
            throw std::runtime_error("Argument is not a number");
        return std::get<double>(value_);
    }

    std::string toString() const {
        if (isString()) return asString();
        return std::to_string(asNumber());
    }

private:
    Value value_;
};

using ArgPtr = std::unique_ptr<Argument>;

} // namespace ppt_cli

#endif // ARGUMENT_H_