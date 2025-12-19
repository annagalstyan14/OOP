#include "TestFramework.h"

// Include all test files
#include "GeometryTests.cpp"
#include "DocumentTests.cpp"
#include "ActionTests.cpp"
#include "ParserTests.cpp"

int main() {
    test::TestFramework::instance().runAll();
    return test::TestFramework::instance().getFailedCount();
}
