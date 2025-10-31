// Shapes/SlideObject.h
#ifndef SLIDE_OBJECT_H_
#define SLIDE_OBJECT_H_

#include <string>

namespace ppt_cli {

enum class ObjectType {
    SHAPE,
    IMAGE,
    TABLE
};

struct SlideObject {
    std::string name;
    ObjectType type;
    std::string color = "black";

    SlideObject(std::string n, ObjectType t, std::string c = "black")
        : name(std::move(n)), type(t), color(std::move(c)) {}
};

} // namespace ppt_cli

#endif // SLIDE_OBJECT_H_