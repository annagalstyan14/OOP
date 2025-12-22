#ifndef SVG_PAINTER_H_
#define SVG_PAINTER_H_

#include "IPainter.h"
#include "SVGCanvas.h"
#include "../Document/Slide.h"
#include "../Document/Circle.h"
#include "../Document/TextObject.h"
#include <memory>
#include <string>

namespace ppt {

class SVGPainter : public IPainter {
private:
    std::unique_ptr<SVGCanvas> canvas_;

public:
    SVGPainter(int width = 800, int height = 600);
    
    void paint(const SlideObject& object) override;
    void paintSlide(const Slide& slide) override;
    
    bool saveToFile(const std::string& filename) override;
    std::string getOutput() const override;
    
    void clear() override;
    
    SVGCanvas* getCanvas() { return canvas_.get(); }
};

} // namespace ppt

#endif // SVG_PAINTER_H_
