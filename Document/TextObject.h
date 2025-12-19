#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "BaseSlideObject.h"

namespace ppt {

class TextObject : public BaseSlideObject {
private:
    std::string content_;
    std::string fontFamily_ = "Arial";
    int fontSize_ = 12;
    std::string alignment_ = "left";

public:
    TextObject(int id, const std::string& name, const std::string& content = "",
               const std::string& color = "black");
    TextObject(const TextObject& other);
    
    std::unique_ptr<SlideObject> clone() const override;
    
    const std::string& getContent() const;
    void setContent(const std::string& content);
    
    const std::string& getFontFamily() const;
    void setFontFamily(const std::string& font);
    
    int getFontSize() const;
    void setFontSize(int size);
    
    const std::string& getAlignment() const;
    void setAlignment(const std::string& align);
    
    void display(std::ostream& os = std::cout) const override;
};

} // namespace ppt

#endif // TEXT_OBJECT_H_
