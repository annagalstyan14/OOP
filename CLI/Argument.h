#ifndef CLI_ARGUMENT_H_
#define CLI_ARGUMENT_H_

#include <string>
#include <variant>
#include <memory>
#include <stdexcept>

namespace ppt_cli {

class Argument {
public:
    using Value = std::variant<std::string, double>;
    
    explicit Argument(const std::string& s);
    explicit Argument(double d);
    
    bool isString() const;
    bool isNumber() const;
    
    const std::string& asString() const;
    double asNumber() const;

private:
    Value value_;
};

using ArgPtr = std::unique_ptr<Argument>;

} // namespace ppt_cli

#endif // CLI_ARGUMENT_H_
