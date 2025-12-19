#include "TestFramework.h"
#include "../Action/Editor.h"
#include "../Action/SlideActions.h"
#include "../Action/ShapeActions.h"

using namespace ppt;
using namespace test;

// ========== Editor Tests ==========

TEST(Editor_Constructor) {
    Presentation pres;
    Editor editor(pres);
    
    ASSERT_FALSE(editor.canUndo());
    ASSERT_FALSE(editor.canRedo());
    ASSERT_EQ(0u, editor.undoStackSize());
    ASSERT_EQ(0u, editor.redoStackSize());
}

TEST(Editor_ExecuteAction) {
    Presentation pres;
    Editor editor(pres);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Test Slide"));
    
    ASSERT_EQ(1u, pres.size());
    ASSERT_TRUE(editor.canUndo());
    ASSERT_FALSE(editor.canRedo());
    ASSERT_EQ(1u, editor.undoStackSize());
}

TEST(Editor_Undo) {
    Presentation pres;
    Editor editor(pres);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Test"));
    ASSERT_EQ(1u, pres.size());
    
    ASSERT_TRUE(editor.undo());
    ASSERT_EQ(0u, pres.size());
    ASSERT_FALSE(editor.canUndo());
    ASSERT_TRUE(editor.canRedo());
}

TEST(Editor_Redo) {
    Presentation pres;
    Editor editor(pres);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Test"));
    editor.undo();
    
    ASSERT_EQ(0u, pres.size());
    ASSERT_TRUE(editor.redo());
    ASSERT_EQ(1u, pres.size());
}

TEST(Editor_UndoEmptyStack) {
    Presentation pres;
    Editor editor(pres);
    
    ASSERT_FALSE(editor.undo());
}

TEST(Editor_RedoEmptyStack) {
    Presentation pres;
    Editor editor(pres);
    
    ASSERT_FALSE(editor.redo());
}

TEST(Editor_NewActionClearsRedoStack) {
    Presentation pres;
    Editor editor(pres);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 1"));
    editor.undo();
    ASSERT_TRUE(editor.canRedo());
    
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 2"));
    ASSERT_FALSE(editor.canRedo());
}

TEST(Editor_MultipleUndoRedo) {
    Presentation pres;
    Editor editor(pres);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 1"));
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 2"));
    editor.executeAction(std::make_unique<AddSlideAction>("Slide 3"));
    
    ASSERT_EQ(3u, pres.size());
    ASSERT_EQ(3u, editor.undoStackSize());
    
    editor.undo();
    editor.undo();
    
    ASSERT_EQ(1u, pres.size());
    ASSERT_EQ(2u, editor.redoStackSize());
    
    editor.redo();
    
    ASSERT_EQ(2u, pres.size());
    ASSERT_EQ(1u, editor.redoStackSize());
}

TEST(Editor_ClearHistory) {
    Presentation pres;
    Editor editor(pres);
    
    editor.executeAction(std::make_unique<AddSlideAction>("Test"));
    editor.undo();
    
    ASSERT_TRUE(editor.canRedo());
    
    editor.clearHistory();
    
    ASSERT_FALSE(editor.canUndo());
    ASSERT_FALSE(editor.canRedo());
}

// ========== AddSlideAction Tests ==========

TEST(AddSlideAction_Execute) {
    Presentation pres;
    AddSlideAction action("Test Slide");
    
    action.execute(pres);
    
    ASSERT_EQ(1u, pres.size());
    ASSERT_EQ("Test Slide", pres.getSlideAt(0)->getTitle());
}

TEST(AddSlideAction_Undo) {
    Presentation pres;
    AddSlideAction action("Test Slide");
    
    action.execute(pres);
    ASSERT_EQ(1u, pres.size());
    
    action.undo(pres);
    ASSERT_EQ(0u, pres.size());
}

TEST(AddSlideAction_GetDescription) {
    AddSlideAction action("My Slide");
    ASSERT_EQ("Add slide: My Slide", action.getDescription());
}

// ========== RemoveSlideAction Tests ==========

TEST(RemoveSlideAction_Execute) {
    Presentation pres;
    pres.addSlide("Slide 1");
    Slide* slide2 = pres.addSlide("Slide 2");
    int id = slide2->getId();
    pres.addSlide("Slide 3");
    
    RemoveSlideAction action(id);
    action.execute(pres);
    
    ASSERT_EQ(2u, pres.size());
    ASSERT_TRUE(pres.getSlide(id) == nullptr);
}

TEST(RemoveSlideAction_Undo) {
    Presentation pres;
    pres.addSlide("Slide 1");
    Slide* slide2 = pres.addSlide("Slide 2");
    int id = slide2->getId();
    
    RemoveSlideAction action(id);
    action.execute(pres);
    ASSERT_EQ(1u, pres.size());
    
    action.undo(pres);
    ASSERT_EQ(2u, pres.size());
}

TEST(RemoveSlideAction_NonexistentSlide) {
    Presentation pres;
    pres.addSlide("Test");
    
    RemoveSlideAction action(999);
    ASSERT_THROW(action.execute(pres), std::runtime_error);
}

// ========== AddShapeAction Tests ==========

TEST(AddShapeAction_Execute) {
    Presentation pres;
    pres.addSlide("Test");
    
    AddShapeAction action("Rect", ObjectType::RECTANGLE, "blue");
    action.execute(pres);
    
    ASSERT_EQ(1u, pres.getSlideAt(0)->objectCount());
}

TEST(AddShapeAction_NoSlide) {
    Presentation pres;
    AddShapeAction action("Rect", ObjectType::RECTANGLE, "blue");
    
    ASSERT_THROW(action.execute(pres), std::runtime_error);
}

TEST(AddShapeAction_Undo) {
    Presentation pres;
    pres.addSlide("Test");
    
    AddShapeAction action("Rect", ObjectType::RECTANGLE, "blue");
    action.execute(pres);
    ASSERT_EQ(1u, pres.getSlideAt(0)->objectCount());
    
    action.undo(pres);
    ASSERT_EQ(0u, pres.getSlideAt(0)->objectCount());
}

// ========== RemoveShapeAction Tests ==========

TEST(RemoveShapeAction_Execute) {
    Presentation pres;
    Slide* slide = pres.addSlide("Test");
    SlideObject* obj = slide->addObject(ObjectType::RECTANGLE, "Rect", "blue");
    int id = obj->getId();
    
    RemoveShapeAction action(id);
    action.execute(pres);
    
    ASSERT_EQ(0u, slide->objectCount());
}

TEST(RemoveShapeAction_Undo) {
    Presentation pres;
    Slide* slide = pres.addSlide("Test");
    SlideObject* obj = slide->addObject(ObjectType::RECTANGLE, "Rect", "blue");
    int id = obj->getId();
    
    RemoveShapeAction action(id);
    action.execute(pres);
    ASSERT_EQ(0u, slide->objectCount());
    
    action.undo(pres);
    ASSERT_EQ(1u, slide->objectCount());
}

TEST(RemoveShapeAction_NoSlide) {
    Presentation pres;
    RemoveShapeAction action(1);
    
    ASSERT_THROW(action.execute(pres), std::runtime_error);
}
