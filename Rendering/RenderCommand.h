#ifndef RENDER_COMMAND_H_
#define RENDER_COMMAND_H_

#include "../Document/Presentation.h"
#include "../Action/Editor.h"
#include "IPainter.h"
#include <string>
#include <memory>

namespace ppt {

class RenderCommand {
public:
    enum class RenderTarget {
        SLIDE,
        PRESENTATION,
        OBJECT
    };

private:
    std::shared_ptr<IPainter> painter_;
    std::string outputPath_;
    RenderTarget target_;
    int targetId_ = -1;

public:
    RenderCommand(std::shared_ptr<IPainter> painter, 
                  const std::string& outputPath,
                  RenderTarget target = RenderTarget::SLIDE);
    
    void setTargetId(int id);
    
    void execute(const Presentation& pres);
    void renderSlide(const Slide& slide, const std::string& filename);
    void renderPresentation(const Presentation& pres);
    void renderObject(const SlideObject& object, const std::string& filename);
    
    const std::string& getOutputPath() const { return outputPath_; }
    void setOutputPath(const std::string& path) { outputPath_ = path; }
};

} // namespace ppt

#endif // RENDER_COMMAND_H_
