// Command/ICommand.h
#ifndef ICOMMAND_H_
#define ICOMMAND_H_

namespace ppt_cli {

// Forward declaration – we only need a reference, not the full definition
class Presentation;

class ICommand {
public:
    virtual ~ICommand() = default;

    // ONE parameter: the presentation that the command will modify
    virtual void execute(Presentation& pres) = 0;
};

} // namespace ppt_cli

#endif // ICOMMAND_H_