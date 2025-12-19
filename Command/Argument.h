#ifndef ARGUMENT_H_
#define ARGUMENT_H_

#include <string>
#include <variant>
#include <memory>
#include <stdexcept>

namespace ppt {

class Argument {
public:
    using Value = std::variant<std::string, double>;
    
    explicit Argument(const std::string& s);
    explicit Argument(double d);
    
    bool isString() const;
    bool isNumber() const;
    
    const std::string& asString() const;
    double asNumber() const;
    int asInt() const;

private:
    Value value_;
};

using ArgPtr = std::unique_ptr<Argument>;

} // namespace ppt

#endif // ARGUMENT_H_
