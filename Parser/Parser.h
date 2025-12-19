#ifndef PARSER_H_
#define PARSER_H_

#include "Tokenizer.h"
#include "../Command/ICommand.h"
#include "../Command/CommandFactory.h"
#include <memory>
#include <string>

namespace ppt {

class Parser {
public:
    Parser(const std::string& input, CommandFactory& factory);
    
    std::unique_ptr<ICommand> parse();
    const std::string& getError() const { return error_; }
    bool hasError() const { return !error_.empty(); }
    
private:
    std::unique_ptr<Tokenizer> tokenizer_;
    CommandFactory& factory_;
    std::string error_;
};

} // namespace ppt

#endif // PARSER_H_
