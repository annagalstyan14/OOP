#include "SlideRenderer.h"
#include <sstream>
#include <fstream>
#include <iostream>

namespace ppt_cli {

SlideRenderer::SlideRenderer(std::unique_ptr<IShapeRenderer> shapeRenderer,
                             int width, int height)
    : shapeRenderer_(std::move(shapeRenderer))
    , width_(width)
    , height_(height) {}

std::string SlideRenderer::render(const Slide& slide) const {
    std::ostringstream svg;
    
    svg << generateHeader();
    svg << generateBackground();
    
    // Add slide title
    svg << "  <text x=\"" << width_ / 2 << "\" y=\"40\" "
        << "text-anchor=\"middle\" font-size=\"24\" font-weight=\"bold\" fill=\"black\">"
        << slide.getTitle() << "</text>\n";
    
    // Render shapes using the shape renderer
    for (const auto& shape : slide.getShapes()) {
        svg << shapeRenderer_->render(*shape);
    }
    
    // Render text areas
    int yOffset = 80;
    for (const auto& [area, text] : slide.getTextAreas()) {
        svg << renderTextArea(area, text, yOffset);
        yOffset += 30;
    }
    
    svg << generateFooter();
    
    return svg.str();
}

bool SlideRenderer::exportToFile(const Slide& slide, const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not create file " << filename << "\n";
        return false;
    }
    
    file << render(slide);
    return true;
}

bool SlideRenderer::exportPresentation(const std::vector<Slide>& slides, 
                                        const std::string& baseFilename) const {
    bool allSuccess = true;
    
    for (size_t i = 0; i < slides.size(); ++i) {
        std::string filename = baseFilename + "_" + std::to_string(i + 1) + ".svg";
        if (!exportToFile(slides[i], filename)) {
            allSuccess = false;
        } else {
            std::cout << "Exported: " << filename << "\n";
        }
    }
    
    return allSuccess;
}

std::string SlideRenderer::generateHeader() const {
    std::ostringstream header;
    header << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    header << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
           << "width=\"" << width_ << "\" height=\"" << height_ << "\" "
           << "viewBox=\"0 0 " << width_ << " " << height_ << "\">\n";
    return header.str();
}

std::string SlideRenderer::generateFooter() const {
    return "</svg>\n";
}

std::string SlideRenderer::generateBackground() const {
    std::ostringstream bg;
    bg << "  <rect width=\"100%\" height=\"100%\" fill=\"" << backgroundColor_ << "\"/>\n";
    bg << "  <rect x=\"1\" y=\"1\" width=\"" << width_ - 2 << "\" height=\"" << height_ - 2 
       << "\" fill=\"none\" stroke=\"#ccc\" stroke-width=\"1\"/>\n";
    return bg.str();
}

std::string SlideRenderer::renderTextArea(const std::string& area, 
                                           const TextArea& text, 
                                           int yOffset) const {
    std::ostringstream textSvg;
    
    textSvg << "  <text x=\"50\" y=\"" << yOffset << "\" "
            << "font-family=\"" << text.fontFamily << "\" "
            << "font-size=\"" << text.fontSize << "\" "
            << "fill=\"" << text.color << "\">"
            << "[" << area << "]: " << text.content << "</text>\n";
    
    return textSvg.str();
}

} // namespace ppt_cli
