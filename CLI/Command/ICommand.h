#ifndef ICOMMAND_H_
#define ICOMMAND_H_

namespace ppt_cli {

class Presentation;
class Editor;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Presentation& pres, Editor& editor) = 0;
};

} // namespace ppt_cli

#endif // ICOMMAND_H_
