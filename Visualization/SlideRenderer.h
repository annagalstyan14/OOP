#ifndef SLIDE_RENDERER_H_
#define SLIDE_RENDERER_H_

#include "../Document/Slide.h"
#include "IShapeRenderer.h"
#include <string>
#include <memory>
#include <vector>

namespace ppt_cli {

class SlideRenderer {
public:
    static constexpr int DEFAULT_WIDTH = 800;
    static constexpr int DEFAULT_HEIGHT = 600;
    
    SlideRenderer(std::unique_ptr<IShapeRenderer> shapeRenderer,
                  int width = DEFAULT_WIDTH, 
                  int height = DEFAULT_HEIGHT);
    
    // Render a single slide to SVG
    std::string render(const Slide& slide) const;
    
    // Export a single slide to file
    bool exportToFile(const Slide& slide, const std::string& filename) const;
    
    // Export all slides
    bool exportPresentation(const std::vector<Slide>& slides, 
                           const std::string& baseFilename) const;
    
    // Setters
    void setWidth(int w) { width_ = w; }
    void setHeight(int h) { height_ = h; }
    void setBackgroundColor(const std::string& color) { backgroundColor_ = color; }

private:
    std::string generateHeader() const;
    std::string generateFooter() const;
    std::string generateBackground() const;
    std::string renderTextArea(const std::string& area, const TextArea& text, int yOffset) const;
    
    std::unique_ptr<IShapeRenderer> shapeRenderer_;
    int width_;
    int height_;
    std::string backgroundColor_ = "white";
};

} // namespace ppt_cli

#endif // SLIDE_RENDERER_H_
