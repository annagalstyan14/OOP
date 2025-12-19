#ifndef RENDER_COMMAND_H_
#define RENDER_COMMAND_H_

#include "../Document/Presentation.h"
#include "SVGPainter.h"
#include <string>
#include <iostream>

namespace ppt {

class RenderCommand {
public:
    static bool exportSlide(const Slide& slide, const std::string& filename, 
                            int width = 800, int height = 600) {
        SVGPainter painter(width, height);
        painter.paintSlide(slide);
        return painter.saveToFile(filename);
    }
    
    static bool exportPresentation(const Presentation& pres, const std::string& baseName,
                                   int width = 800, int height = 600) {
        bool success = true;
        
        for (size_t i = 0; i < pres.size(); ++i) {
            const Slide* slide = pres.getSlideAt(i);
            if (!slide) continue;
            
            std::string filename = baseName + "_" + std::to_string(i + 1) + ".svg";
            
            if (exportSlide(*slide, filename, width, height)) {
                std::cout << "Exported: " << filename << "\n";
            } else {
                std::cerr << "Failed to export: " << filename << "\n";
                success = false;
            }
        }
        
        return success;
    }
};

} // namespace ppt

#endif // RENDER_COMMAND_H_
