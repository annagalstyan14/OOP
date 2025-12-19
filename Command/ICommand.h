#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include "../Document/Presentation.h"
#include "../Action/Editor.h"

namespace ppt {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Presentation& pres, Editor& editor) = 0;
};

} // namespace ppt

#endif // ICOMMAND_H_
