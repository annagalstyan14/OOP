#ifndef IPAINTER_H_
#define IPAINTER_H_

#include "../Document/SlideObject.h"
#include "../Document/Slide.h"
#include <string>

namespace ppt {

class IPainter {
public:
    virtual ~IPainter() = default;
    
    virtual void paint(const SlideObject& object) = 0;
    virtual void paintSlide(const Slide& slide) = 0;
    
    virtual bool saveToFile(const std::string& filename) = 0;
    virtual std::string getOutput() const = 0;
    
    virtual void clear() = 0;
};

} // namespace ppt

#endif // IPAINTER_H_
