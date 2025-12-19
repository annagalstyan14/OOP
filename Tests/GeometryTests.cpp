#include "TestFramework.h"
#include "../Geometry/Geometry.h"

using namespace ppt;
using namespace test;

// ========== Point Tests ==========

TEST(Point_DefaultConstructor) {
    Point p;
    ASSERT_DOUBLE_EQ(0.0, p.x);
    ASSERT_DOUBLE_EQ(0.0, p.y);
}

TEST(Point_ParameterizedConstructor) {
    Point p(10.5, 20.5);
    ASSERT_DOUBLE_EQ(10.5, p.x);
    ASSERT_DOUBLE_EQ(20.5, p.y);
}

TEST(Point_Equality) {
    Point p1(10.0, 20.0);
    Point p2(10.0, 20.0);
    Point p3(10.0, 21.0);
    
    ASSERT_TRUE(p1 == p2);
    ASSERT_FALSE(p1 == p3);
    ASSERT_TRUE(p1 != p3);
}

// ========== Geometry Tests ==========

TEST(Geometry_DefaultConstructor) {
    Geometry g;
    ASSERT_DOUBLE_EQ(0.0, g.topLeft.x);
    ASSERT_DOUBLE_EQ(0.0, g.topLeft.y);
    ASSERT_DOUBLE_EQ(100.0, g.bottomRight.x);
    ASSERT_DOUBLE_EQ(100.0, g.bottomRight.y);
}

TEST(Geometry_ParameterizedConstructor) {
    Geometry g(10, 20, 110, 120);
    ASSERT_DOUBLE_EQ(10.0, g.getX());
    ASSERT_DOUBLE_EQ(20.0, g.getY());
    ASSERT_DOUBLE_EQ(100.0, g.getWidth());
    ASSERT_DOUBLE_EQ(100.0, g.getHeight());
}

TEST(Geometry_GetWidthHeight) {
    Geometry g(0, 0, 200, 150);
    ASSERT_DOUBLE_EQ(200.0, g.getWidth());
    ASSERT_DOUBLE_EQ(150.0, g.getHeight());
}

TEST(Geometry_Move) {
    Geometry g(10, 10, 50, 50);
    g.move(5, 10);
    
    ASSERT_DOUBLE_EQ(15.0, g.topLeft.x);
    ASSERT_DOUBLE_EQ(20.0, g.topLeft.y);
    ASSERT_DOUBLE_EQ(55.0, g.bottomRight.x);
    ASSERT_DOUBLE_EQ(60.0, g.bottomRight.y);
    ASSERT_DOUBLE_EQ(40.0, g.getWidth());  // Width preserved
    ASSERT_DOUBLE_EQ(40.0, g.getHeight()); // Height preserved
}

TEST(Geometry_MoveNegative) {
    Geometry g(100, 100, 200, 200);
    g.move(-50, -30);
    
    ASSERT_DOUBLE_EQ(50.0, g.topLeft.x);
    ASSERT_DOUBLE_EQ(70.0, g.topLeft.y);
}

TEST(Geometry_Resize) {
    Geometry g(0, 0, 100, 100);
    g.resize(50, 25);
    
    ASSERT_DOUBLE_EQ(150.0, g.getWidth());
    ASSERT_DOUBLE_EQ(125.0, g.getHeight());
}

TEST(Geometry_ResizeMinimum) {
    Geometry g(0, 0, 100, 100);
    g.resize(-200, -200);  // Would result in negative dimensions
    
    ASSERT_TRUE(g.getWidth() >= 1.0);
    ASSERT_TRUE(g.getHeight() >= 1.0);
}

TEST(Geometry_SetPosition) {
    Geometry g(10, 10, 50, 50);
    double originalWidth = g.getWidth();
    double originalHeight = g.getHeight();
    
    g.setPosition(100, 200);
    
    ASSERT_DOUBLE_EQ(100.0, g.getX());
    ASSERT_DOUBLE_EQ(200.0, g.getY());
    ASSERT_DOUBLE_EQ(originalWidth, g.getWidth());
    ASSERT_DOUBLE_EQ(originalHeight, g.getHeight());
}

TEST(Geometry_SetSize) {
    Geometry g(10, 10, 50, 50);
    g.setSize(200, 150);
    
    ASSERT_DOUBLE_EQ(200.0, g.getWidth());
    ASSERT_DOUBLE_EQ(150.0, g.getHeight());
    ASSERT_DOUBLE_EQ(10.0, g.getX());  // Position preserved
    ASSERT_DOUBLE_EQ(10.0, g.getY());
}

TEST(Geometry_Contains) {
    Geometry g(10, 10, 100, 100);
    
    ASSERT_TRUE(g.contains(50, 50));   // Inside
    ASSERT_TRUE(g.contains(10, 10));   // On corner
    ASSERT_TRUE(g.contains(100, 100)); // On corner
    ASSERT_FALSE(g.contains(5, 50));   // Outside left
    ASSERT_FALSE(g.contains(150, 50)); // Outside right
    ASSERT_FALSE(g.contains(50, 5));   // Outside top
    ASSERT_FALSE(g.contains(50, 150)); // Outside bottom
}

TEST(Geometry_Equality) {
    Geometry g1(10, 20, 100, 200);
    Geometry g2(10, 20, 100, 200);
    Geometry g3(10, 20, 100, 201);
    
    ASSERT_TRUE(g1 == g2);
    ASSERT_FALSE(g1 == g3);
}
