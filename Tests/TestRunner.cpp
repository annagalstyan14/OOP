#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "../Geometry/Geometry.h"
#include "../Document/Presentation.h"
#include "../Document/Rectangle.h"
#include "../Document/Circle.h"
#include "../Document/Line.h"
#include "../Document/TextObject.h"
#include "../Document/JSONSerializer.h"
#include "../Action/Editor.h"
#include "../Action/SlideActions.h"
#include "../Action/ShapeActions.h"
#include "../Parser/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Command/CommandFactory.h"
#include "../Rendering/SVGPainter.h"
#include "../Visualization/SvgShapeRenderer.h"
#include "../Visualization/SlideRenderer.h"

using namespace ppt;

int passed = 0;
int failed = 0;

#define TEST(name) \
    std::cout << "[ RUN ] " << #name << "\n"; \
    try { test_##name(); passed++; std::cout << "[  OK ] " << #name << "\n"; } \
    catch (const std::exception& e) { failed++; std::cout << "[FAIL ] " << #name << ": " << e.what() << "\n"; }

#define ASSERT(cond) if (!(cond)) throw std::runtime_error("Assertion failed: " #cond)

// === Geometry ===
void test_Geometry() {
    Point p(10, 20);
    ASSERT(p.x == 10 && p.y == 20);
    
    Geometry g(10, 20, 110, 120);
    ASSERT(g.getX() == 10 && g.getY() == 20);
    ASSERT(g.getWidth() == 100 && g.getHeight() == 100);
    
    g.move(5, 5);
    ASSERT(g.getX() == 15 && g.getY() == 25);
    
    g.setPosition(0, 0);
    ASSERT(g.getX() == 0 && g.getY() == 0);
    
    g.setSize(200, 150);
    ASSERT(g.getWidth() == 200 && g.getHeight() == 150);
    
    ASSERT(g.contains(100, 75));
    ASSERT(!g.contains(300, 300));
}

// === SlideObjects ===
void test_SlideObjects() {
    Rectangle rect(1, "Rect", "blue");
    ASSERT(rect.getId() == 1);
    ASSERT(rect.getName() == "Rect");
    ASSERT(rect.getColor() == "blue");
    ASSERT(rect.getType() == ObjectType::RECTANGLE);
    
    auto rectClone = rect.clone();
    ASSERT(rectClone->getName() == "Rect");
    
    Circle circle(2, "Circle", "red");
    ASSERT(circle.getType() == ObjectType::CIRCLE);
    circle.setGeometry(Geometry(0, 0, 100, 100));
    ASSERT(circle.getRadius() == 50);
    Point center = circle.getCenter();
    ASSERT(center.x == 50 && center.y == 50);
    
    Line line(3, "Line", "black");
    ASSERT(line.getType() == ObjectType::LINE);
    line.setStart(Point(0, 0));
    line.setEnd(Point(100, 100));
    ASSERT(line.getStart().x == 0 && line.getEnd().x == 100);
    
    TextObject text(4, "Text", "Hello World", "green");
    ASSERT(text.getType() == ObjectType::TEXT);
    ASSERT(text.getContent() == "Hello World");
    text.setFontFamily("Arial");
    text.setFontSize(24);
    text.setAlignment("center");
    ASSERT(text.getFontFamily() == "Arial");
    ASSERT(text.getFontSize() == 24);
    ASSERT(text.getAlignment() == "center");
    
    ASSERT(SlideObject::typeToString(ObjectType::RECTANGLE) == "rectangle");
    ASSERT(SlideObject::stringToType("circle") == ObjectType::CIRCLE);
}

// === Slide ===
void test_Slide() {
    Slide slide(1, "Test Slide");
    ASSERT(slide.getId() == 1);
    ASSERT(slide.getTitle() == "Test Slide");
    ASSERT(slide.objectCount() == 0);
    
    slide.addObject(ObjectType::RECTANGLE, "Rect", "blue");
    slide.addObject(ObjectType::CIRCLE, "Circle", "red");
    slide.addObject(ObjectType::LINE, "Line", "black");
    ASSERT(slide.objectCount() == 3);
    
    SlideObject* obj = slide.getObject(1);
    ASSERT(obj != nullptr && obj->getName() == "Rect");
    
    ASSERT(slide.removeObject(1));
    ASSERT(slide.objectCount() == 2);
    ASSERT(slide.getObject(1) == nullptr);
    
    TextObject* text = slide.addText("Title", "Hello", "black");
    ASSERT(text != nullptr && text->getContent() == "Hello");
    
    Slide copy(slide);
    ASSERT(copy.objectCount() == slide.objectCount());
    slide.addObject(ObjectType::RECTANGLE, "New", "yellow");
    ASSERT(copy.objectCount() != slide.objectCount());
}

// === Presentation ===
void test_Presentation() {
    Presentation pres;
    ASSERT(pres.empty() && pres.size() == 0);
    
    Slide* s1 = pres.addSlide("Slide 1");
    int s1_id = s1->getId();
    
    pres.addSlide("Slide 2");
    int s2_id = pres.getSlideAt(1)->getId();
    
    pres.addSlide("Slide 3");
    ASSERT(pres.size() == 3);
    
    ASSERT(pres.getSlide(s1_id) != nullptr);
    ASSERT(pres.getSlide(s1_id)->getTitle() == "Slide 1");
    ASSERT(pres.getSlideAt(0)->getTitle() == "Slide 1");
    ASSERT(pres.getSlideAt(1)->getTitle() == "Slide 2");
    
    ASSERT(pres.removeSlide(s2_id));
    ASSERT(pres.size() == 2);
    
    pres.insertSlide(1, "Inserted");
    ASSERT(pres.getSlideAt(1)->getTitle() == "Inserted");
    
    pres.clear();
    ASSERT(pres.empty());
}

// === Editor & Actions ===
void test_EditorActions() {
    Presentation pres;
    Editor editor(pres);
    
    ASSERT(!editor.canUndo() && !editor.canRedo());
    ASSERT(editor.undoStackSize() == 0);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 1"));
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 2"));
    ASSERT(pres.size() == 2);
    ASSERT(editor.canUndo() && editor.undoStackSize() == 2);
    
    editor.undo();
    ASSERT(pres.size() == 1);
    ASSERT(editor.canRedo());
    
    editor.redo();
    ASSERT(pres.size() == 2);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 3"));
    ASSERT(!editor.canRedo());
    
    editor.executeAction(std::make_unique<AddShapeAction>("Rect", ObjectType::RECTANGLE, "blue"));
    ASSERT(pres.getSlideAt(pres.size() - 1)->objectCount() == 1);
    
    editor.undo();
    ASSERT(pres.getSlideAt(pres.size() - 1)->objectCount() == 0);
    
    editor.clearHistory();
    ASSERT(!editor.canUndo() && !editor.canRedo());
}

// === Tokenizer ===
void test_Tokenizer() {
    Tokenizer tok1("add slide \"Hello World\" 123");
    
    Token t1 = tok1.getToken();
    ASSERT(t1.type == TokenType::KEYWORD && t1.keyword == Keyword::ADD);
    
    Token t2 = tok1.getToken();
    ASSERT(t2.keyword == Keyword::SLIDE);
    
    Token t3 = tok1.getToken();
    ASSERT(t3.type == TokenType::STRING && t3.value == "Hello World");
    
    Token t4 = tok1.getToken();
    ASSERT(t4.type == TokenType::NUMBER && t4.value == "123");
    
    Token t5 = tok1.getToken();
    ASSERT(t5.type == TokenType::END);
    
    Tokenizer tok2("ADD REMOVE LIST SAVE LOAD");
    ASSERT(tok2.getToken().keyword == Keyword::ADD);
    ASSERT(tok2.getToken().keyword == Keyword::REMOVE);
    ASSERT(tok2.getToken().keyword == Keyword::LIST);
    ASSERT(tok2.getToken().keyword == Keyword::SAVE);
    ASSERT(tok2.getToken().keyword == Keyword::LOAD);
    
    Tokenizer tok3("-42.5");
    Token neg = tok3.getToken();
    ASSERT(neg.type == TokenType::NUMBER && neg.value == "-42.5");
}

// === Parser & CommandFactory ===
void test_Parser() {
    CommandFactory factory;
    
    ASSERT(factory.hasCommand("add"));
    ASSERT(factory.hasCommand("remove"));
    ASSERT(factory.hasCommand("list"));
    ASSERT(!factory.hasCommand("invalid"));
    
    Parser p1("add slide \"Test\"", factory);
    ASSERT(p1.parse() != nullptr && !p1.hasError());
    
    Parser p2("list slides", factory);
    ASSERT(p2.parse() != nullptr);
    
    Parser p3("remove slide 1", factory);
    ASSERT(p3.parse() != nullptr);
    
    Parser p4("", factory);
    ASSERT(p4.parse() == nullptr && p4.hasError());
    
    Parser p5("unknown command", factory);
    ASSERT(p5.parse() == nullptr);
}

// === JSON Serialization ===
void test_JSONSerialization() {
    Presentation pres;
    Slide* slide = pres.addSlide("Test Slide");
    slide->addObject(ObjectType::RECTANGLE, "Rect", "blue");
    slide->addObject(ObjectType::CIRCLE, "Circle", "red");
    slide->addText("Title", "Hello", "black");
    
    std::string filename = "/tmp/test_pres.json";
    ASSERT(JSONSerializer::save(pres, filename));
    
    Presentation loaded;
    ASSERT(JSONSerializer::load(loaded, filename));
    ASSERT(loaded.size() == 1);
    ASSERT(loaded.getSlideAt(0)->getTitle() == "Test Slide");
    ASSERT(loaded.getSlideAt(0)->objectCount() == 3);
    
    std::remove(filename.c_str());
}

// === SVG Rendering (ppt namespace) ===
void test_SVGPainter() {
    SVGPainter painter(800, 600);
    
    Slide slide(1, "Test Slide");
    slide.addObject(ObjectType::RECTANGLE, "Rect", "blue");
    slide.addObject(ObjectType::CIRCLE, "Circle", "red");
    slide.addObject(ObjectType::LINE, "Line", "black");
    slide.addText("Label", "Hello", "green");
    
    painter.paintSlide(slide);
    std::string svg = painter.getOutput();
    
    ASSERT(svg.find("<svg") != std::string::npos);
    ASSERT(svg.find("</svg>") != std::string::npos);
    ASSERT(svg.find("<rect") != std::string::npos);
    ASSERT(svg.find("<ellipse") != std::string::npos);
    ASSERT(svg.find("<line") != std::string::npos);
    ASSERT(svg.find("<text") != std::string::npos);
    ASSERT(svg.find("Test Slide") != std::string::npos);
    
    std::string filename = "/tmp/test_slide.svg";
    ASSERT(painter.saveToFile(filename));
    
    std::ifstream file(filename);
    ASSERT(file.good());
    file.close();
    std::remove(filename.c_str());
}

// === Visualization (ppt_cli namespace) ===
void test_SlideRenderer() {
    auto shapeRenderer = std::make_unique<ppt_cli::SvgShapeRenderer>();
    ppt_cli::SlideRenderer renderer(std::move(shapeRenderer), 800, 600);
    
    ppt_cli::Slide slide(1, "CLI Slide");
    slide.addShape("Rect", ppt_cli::ShapeType::RECTANGLE, "blue");
    slide.addShape("Ellipse", ppt_cli::ShapeType::ELLIPSE, "red");
    slide.addShape("Line", ppt_cli::ShapeType::LINE, "black");
    slide.addText("title", "Hello World");
    
    std::string svg = renderer.render(slide);
    
    ASSERT(svg.find("<svg") != std::string::npos);
    ASSERT(svg.find("</svg>") != std::string::npos);
    ASSERT(svg.find("CLI Slide") != std::string::npos);
    ASSERT(svg.find("<rect") != std::string::npos);
    ASSERT(svg.find("<ellipse") != std::string::npos);
    ASSERT(svg.find("<line") != std::string::npos);
    ASSERT(svg.find("Hello World") != std::string::npos);
    
    std::string filename = "/tmp/test_cli_slide.svg";
    ASSERT(renderer.exportToFile(slide, filename));
    std::remove(filename.c_str());
}

// === SvgShapeRenderer ===
void test_SvgShapeRenderer() {
    ppt_cli::SvgShapeRenderer renderer;
    
    ppt_cli::Shape rect(1, "Rect", ppt_cli::ShapeType::RECTANGLE, "blue");
    rect.setBoundingBox(ppt_cli::BoundingBox(10, 20, 110, 120));
    std::string rectSvg = renderer.render(rect);
    ASSERT(rectSvg.find("<rect") != std::string::npos);
    ASSERT(rectSvg.find("x=\"10\"") != std::string::npos);
    ASSERT(rectSvg.find("blue") != std::string::npos);
    
    ppt_cli::Shape ellipse(2, "Ellipse", ppt_cli::ShapeType::ELLIPSE, "red");
    ellipse.setBoundingBox(ppt_cli::BoundingBox(0, 0, 100, 50));
    std::string ellipseSvg = renderer.render(ellipse);
    ASSERT(ellipseSvg.find("<ellipse") != std::string::npos);
    ASSERT(ellipseSvg.find("red") != std::string::npos);
    
    ppt_cli::Shape triangle(3, "Tri", ppt_cli::ShapeType::TRIANGLE, "green");
    std::string triSvg = renderer.render(triangle);
    ASSERT(triSvg.find("<polygon") != std::string::npos);
    
    ppt_cli::Shape line(4, "Line", ppt_cli::ShapeType::LINE, "black");
    std::string lineSvg = renderer.render(line);
    ASSERT(lineSvg.find("<line") != std::string::npos);
}

// === Integration Test ===
void test_Integration() {
    Presentation pres;
    Editor editor(pres);
    CommandFactory factory;
    
    Parser p1("add slide \"Intro\"", factory);
    auto cmd1 = p1.parse();
    ASSERT(cmd1 != nullptr);
    cmd1->execute(pres, editor);
    ASSERT(pres.size() == 1);
    
    editor.executeAction(std::make_unique<AddShapeAction>("Box", ObjectType::RECTANGLE, "blue"));
    editor.executeAction(std::make_unique<AddShapeAction>("Ball", ObjectType::CIRCLE, "red"));
    ASSERT(pres.getSlideAt(0)->objectCount() == 2);
    
    std::string jsonFile = "/tmp/integration_test.json";
    ASSERT(JSONSerializer::save(pres, jsonFile));
    
    Presentation loaded;
    ASSERT(JSONSerializer::load(loaded, jsonFile));
    ASSERT(loaded.size() == 1);
    ASSERT(loaded.getSlideAt(0)->objectCount() == 2);
    
    SVGPainter painter(800, 600);
    painter.paintSlide(*loaded.getSlideAt(0));
    std::string svg = painter.getOutput();
    ASSERT(svg.find("Intro") != std::string::npos);
    ASSERT(svg.find("<rect") != std::string::npos);
    ASSERT(svg.find("<ellipse") != std::string::npos);
    
    editor.undo();
    editor.undo();
    ASSERT(pres.getSlideAt(0)->objectCount() == 0);
    
    editor.redo();
    editor.redo();
    ASSERT(pres.getSlideAt(0)->objectCount() == 2);
    
    std::remove(jsonFile.c_str());
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "       PPT-CLI Unit Tests\n";
    std::cout << "========================================\n\n";
    
    TEST(Geometry);
    TEST(SlideObjects);
    TEST(Slide);
    TEST(Presentation);
    TEST(EditorActions);
    TEST(Tokenizer);
    TEST(Parser);
    TEST(JSONSerialization);
    TEST(SVGPainter);
    TEST(SlideRenderer);
    TEST(SvgShapeRenderer);
    TEST(Integration);
    
    std::cout << "\n========================================\n";
    std::cout << "Results: " << passed << " passed, " << failed << " failed\n";
    std::cout << "========================================\n";
    
    return failed;
}