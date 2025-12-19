#include "TestFramework.h"
#include "../Document/Presentation.h"
#include "../Document/Rectangle.h"
#include "../Document/Circle.h"
#include "../Document/Line.h"
#include "../Document/TextObject.h"

using namespace ppt;
using namespace test;

// ========== SlideObject Tests ==========

TEST(SlideObject_TypeToString) {
    ASSERT_EQ("rectangle", SlideObject::typeToString(ObjectType::RECTANGLE));
    ASSERT_EQ("circle", SlideObject::typeToString(ObjectType::CIRCLE));
    ASSERT_EQ("line", SlideObject::typeToString(ObjectType::LINE));
    ASSERT_EQ("text", SlideObject::typeToString(ObjectType::TEXT));
}

TEST(SlideObject_StringToType) {
    ASSERT_TRUE(ObjectType::RECTANGLE == SlideObject::stringToType("rectangle"));
    ASSERT_TRUE(ObjectType::CIRCLE == SlideObject::stringToType("circle"));
    ASSERT_TRUE(ObjectType::CIRCLE == SlideObject::stringToType("ellipse"));
    ASSERT_TRUE(ObjectType::LINE == SlideObject::stringToType("line"));
    ASSERT_TRUE(ObjectType::TEXT == SlideObject::stringToType("text"));
    ASSERT_TRUE(ObjectType::RECTANGLE == SlideObject::stringToType("unknown"));
}

// ========== Rectangle Tests ==========

TEST(Rectangle_Constructor) {
    Rectangle rect(1, "TestRect", "blue");
    
    ASSERT_EQ(1, rect.getId());
    ASSERT_EQ("TestRect", rect.getName());
    ASSERT_EQ("blue", rect.getColor());
    ASSERT_TRUE(ObjectType::RECTANGLE == rect.getType());
}

TEST(Rectangle_Clone) {
    Rectangle rect(1, "Original", "red");
    rect.getGeometry().setPosition(50, 50);
    
    auto clone = rect.clone();
    
    ASSERT_EQ(rect.getId(), clone->getId());
    ASSERT_EQ(rect.getName(), clone->getName());
    ASSERT_EQ(rect.getColor(), clone->getColor());
    ASSERT_TRUE(rect.getGeometry() == clone->getGeometry());
}

TEST(Rectangle_Move) {
    Rectangle rect(1, "Rect", "blue");
    rect.move(10, 20);
    
    ASSERT_DOUBLE_EQ(10.0, rect.getGeometry().getX());
    ASSERT_DOUBLE_EQ(20.0, rect.getGeometry().getY());
}

// ========== Circle Tests ==========

TEST(Circle_Constructor) {
    Circle circle(1, "TestCircle", "green");
    
    ASSERT_EQ(1, circle.getId());
    ASSERT_TRUE(ObjectType::CIRCLE == circle.getType());
}

TEST(Circle_GetCenterAndRadius) {
    Circle circle(1, "TestCircle", "green");
    circle.setGeometry(Geometry(0, 0, 100, 100));
    
    Point center = circle.getCenter();
    ASSERT_DOUBLE_EQ(50.0, center.x);
    ASSERT_DOUBLE_EQ(50.0, center.y);
    ASSERT_DOUBLE_EQ(50.0, circle.getRadius());
}

// ========== Line Tests ==========

TEST(Line_Constructor) {
    Line line(1, "TestLine", "black");
    
    ASSERT_TRUE(ObjectType::LINE == line.getType());
}

TEST(Line_SetStartEnd) {
    Line line(1, "TestLine", "black");
    line.setStart(Point(10, 20));
    line.setEnd(Point(100, 200));
    
    Point start = line.getStart();
    Point end = line.getEnd();
    
    ASSERT_DOUBLE_EQ(10.0, start.x);
    ASSERT_DOUBLE_EQ(20.0, start.y);
    ASSERT_DOUBLE_EQ(100.0, end.x);
    ASSERT_DOUBLE_EQ(200.0, end.y);
}

// ========== TextObject Tests ==========

TEST(TextObject_Constructor) {
    TextObject text(1, "Title", "Hello World", "black");
    
    ASSERT_EQ("Hello World", text.getContent());
    ASSERT_EQ("Arial", text.getFontFamily());
    ASSERT_EQ(12, text.getFontSize());
    ASSERT_EQ("left", text.getAlignment());
}

TEST(TextObject_SetProperties) {
    TextObject text(1, "Title", "Hello");
    
    text.setContent("New Content");
    text.setFontFamily("Times");
    text.setFontSize(24);
    text.setAlignment("center");
    
    ASSERT_EQ("New Content", text.getContent());
    ASSERT_EQ("Times", text.getFontFamily());
    ASSERT_EQ(24, text.getFontSize());
    ASSERT_EQ("center", text.getAlignment());
}

TEST(TextObject_Clone) {
    TextObject text(1, "Title", "Content", "blue");
    text.setFontFamily("Courier");
    text.setFontSize(18);
    
    auto clone = text.clone();
    TextObject* clonedText = dynamic_cast<TextObject*>(clone.get());
    
    ASSERT_TRUE(clonedText != nullptr);
    ASSERT_EQ(text.getContent(), clonedText->getContent());
    ASSERT_EQ(text.getFontFamily(), clonedText->getFontFamily());
    ASSERT_EQ(text.getFontSize(), clonedText->getFontSize());
}

// ========== Slide Tests ==========

TEST(Slide_Constructor) {
    Slide slide(1, "Test Slide");
    
    ASSERT_EQ(1, slide.getId());
    ASSERT_EQ("Test Slide", slide.getTitle());
    ASSERT_EQ(0u, slide.objectCount());
}

TEST(Slide_AddObject) {
    Slide slide(1, "Test");
    
    SlideObject* rect = slide.addObject(ObjectType::RECTANGLE, "Rect1", "blue");
    
    ASSERT_TRUE(rect != nullptr);
    ASSERT_EQ(1u, slide.objectCount());
    ASSERT_EQ("Rect1", rect->getName());
}

TEST(Slide_AddMultipleObjects) {
    Slide slide(1, "Test");
    
    slide.addObject(ObjectType::RECTANGLE, "Rect", "blue");
    slide.addObject(ObjectType::CIRCLE, "Circle", "red");
    slide.addObject(ObjectType::LINE, "Line", "black");
    
    ASSERT_EQ(3u, slide.objectCount());
}

TEST(Slide_GetObject) {
    Slide slide(1, "Test");
    SlideObject* added = slide.addObject(ObjectType::RECTANGLE, "Rect", "blue");
    int id = added->getId();
    
    SlideObject* found = slide.getObject(id);
    ASSERT_TRUE(found != nullptr);
    ASSERT_EQ(id, found->getId());
    
    SlideObject* notFound = slide.getObject(999);
    ASSERT_TRUE(notFound == nullptr);
}

TEST(Slide_RemoveObject) {
    Slide slide(1, "Test");
    SlideObject* obj = slide.addObject(ObjectType::RECTANGLE, "Rect", "blue");
    int id = obj->getId();
    
    ASSERT_EQ(1u, slide.objectCount());
    ASSERT_TRUE(slide.removeObject(id));
    ASSERT_EQ(0u, slide.objectCount());
    ASSERT_FALSE(slide.removeObject(id));  // Already removed
}

TEST(Slide_AddText) {
    Slide slide(1, "Test");
    TextObject* text = slide.addText("Title", "Hello World", "black");
    
    ASSERT_TRUE(text != nullptr);
    ASSERT_EQ("Hello World", text->getContent());
    ASSERT_EQ(1u, slide.objectCount());
}

TEST(Slide_CopyConstructor) {
    Slide original(1, "Original");
    original.addObject(ObjectType::RECTANGLE, "Rect", "blue");
    original.addObject(ObjectType::CIRCLE, "Circle", "red");
    
    Slide copy(original);
    
    ASSERT_EQ(original.getId(), copy.getId());
    ASSERT_EQ(original.getTitle(), copy.getTitle());
    ASSERT_EQ(original.objectCount(), copy.objectCount());
    
    // Modify original, copy should be independent
    original.addObject(ObjectType::LINE, "Line", "black");
    ASSERT_NE(original.objectCount(), copy.objectCount());
}

// ========== Presentation Tests ==========

TEST(Presentation_Empty) {
    Presentation pres;
    
    ASSERT_EQ(0u, pres.size());
    ASSERT_TRUE(pres.empty());
}

TEST(Presentation_AddSlide) {
    Presentation pres;
    
    Slide* slide = pres.addSlide("First Slide");
    
    ASSERT_TRUE(slide != nullptr);
    ASSERT_EQ(1u, pres.size());
    ASSERT_EQ("First Slide", slide->getTitle());
}

TEST(Presentation_AddMultipleSlides) {
    Presentation pres;
    
    pres.addSlide("Slide 1");
    pres.addSlide("Slide 2");
    pres.addSlide("Slide 3");
    
    ASSERT_EQ(3u, pres.size());
}

TEST(Presentation_GetSlide) {
    Presentation pres;
    Slide* added = pres.addSlide("Test");
    int id = added->getId();
    
    Slide* found = pres.getSlide(id);
    ASSERT_TRUE(found != nullptr);
    ASSERT_EQ(id, found->getId());
    
    Slide* notFound = pres.getSlide(999);
    ASSERT_TRUE(notFound == nullptr);
}

TEST(Presentation_GetSlideAt) {
    Presentation pres;
    pres.addSlide("Slide 0");
    pres.addSlide("Slide 1");
    pres.addSlide("Slide 2");
    
    ASSERT_EQ("Slide 0", pres.getSlideAt(0)->getTitle());
    ASSERT_EQ("Slide 1", pres.getSlideAt(1)->getTitle());
    ASSERT_EQ("Slide 2", pres.getSlideAt(2)->getTitle());
    ASSERT_TRUE(pres.getSlideAt(3) == nullptr);
}

TEST(Presentation_RemoveSlide) {
    Presentation pres;
    Slide* slide = pres.addSlide("Test");
    int id = slide->getId();
    
    ASSERT_EQ(1u, pres.size());
    ASSERT_TRUE(pres.removeSlide(id));
    ASSERT_EQ(0u, pres.size());
    ASSERT_FALSE(pres.removeSlide(id));  // Already removed
}

TEST(Presentation_RemoveSlideAt) {
    Presentation pres;
    pres.addSlide("Slide 0");
    pres.addSlide("Slide 1");
    pres.addSlide("Slide 2");
    
    ASSERT_TRUE(pres.removeSlideAt(1));
    ASSERT_EQ(2u, pres.size());
    ASSERT_EQ("Slide 0", pres.getSlideAt(0)->getTitle());
    ASSERT_EQ("Slide 2", pres.getSlideAt(1)->getTitle());
}

TEST(Presentation_Clear) {
    Presentation pres;
    pres.addSlide("Slide 1");
    pres.addSlide("Slide 2");
    
    ASSERT_EQ(2u, pres.size());
    pres.clear();
    ASSERT_EQ(0u, pres.size());
    ASSERT_TRUE(pres.empty());
}

TEST(Presentation_InsertSlide) {
    Presentation pres;
    pres.addSlide("Slide 0");
    pres.addSlide("Slide 2");
    
    pres.insertSlide(1, "Slide 1");
    
    ASSERT_EQ(3u, pres.size());
    ASSERT_EQ("Slide 0", pres.getSlideAt(0)->getTitle());
    ASSERT_EQ("Slide 1", pres.getSlideAt(1)->getTitle());
    ASSERT_EQ("Slide 2", pres.getSlideAt(2)->getTitle());
}
