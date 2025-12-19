#include "TextObject.h"

namespace ppt {

TextObject::TextObject(int id, const std::string& name, const std::string& content,
                       const std::string& color)
    : BaseSlideObject(id, name, ObjectType::TEXT, color)
    , content_(content) {}

TextObject::TextObject(const TextObject& other) 
    : BaseSlideObject(other)
    , content_(other.content_)
    , fontFamily_(other.fontFamily_)
    , fontSize_(other.fontSize_)
    , alignment_(other.alignment_) {}

std::unique_ptr<SlideObject> TextObject::clone() const {
    return std::make_unique<TextObject>(*this);
}

const std::string& TextObject::getContent() const { return content_; }
void TextObject::setContent(const std::string& content) { content_ = content; }

const std::string& TextObject::getFontFamily() const { return fontFamily_; }
void TextObject::setFontFamily(const std::string& font) { fontFamily_ = font; }

int TextObject::getFontSize() const { return fontSize_; }
void TextObject::setFontSize(int size) { fontSize_ = size; }

const std::string& TextObject::getAlignment() const { return alignment_; }
void TextObject::setAlignment(const std::string& align) { alignment_ = align; }

void TextObject::display(std::ostream& os) const {
    os << "text #" << id_ << ": " << name_ 
       << " \"" << content_ << "\" "
       << "(Font: " << fontFamily_ << " " << fontSize_ << "pt, "
       << "Color: " << color_ << ", Align: " << alignment_ << ")\n";
}

} // namespace ppt
