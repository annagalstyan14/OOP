#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include "../../Action/Editor.h" // Include the full definition of Editor

namespace ppt_cli {

class Presentation;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Presentation& pres, Editor& editor) = 0;
};

} // namespace ppt_cli

#endif // ICOMMAND_H_
