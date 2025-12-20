#ifndef CLI_ICOMMAND_H_
#define CLI_ICOMMAND_H_

#include "../../Document/Presentation.h"
#include "../../Action/Editor.h"

namespace ppt_cli {

// Use ppt namespace types
using ppt::Presentation;
using ppt::Editor;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Presentation& pres, Editor& editor) = 0;
};

} // namespace ppt_cli

#endif // CLI_ICOMMAND_H_
