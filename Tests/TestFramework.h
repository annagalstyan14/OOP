#ifndef TEST_FRAMEWORK_H_
#define TEST_FRAMEWORK_H_

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <cmath>

namespace test {

class TestFramework {
private:
    struct TestCase {
        std::string name;
        std::function<void()> func;
    };
    
    std::vector<TestCase> tests_;
    int passed_ = 0;
    int failed_ = 0;
    std::string currentTest_;

public:
    static TestFramework& instance() {
        static TestFramework framework;
        return framework;
    }
    
    void addTest(const std::string& name, std::function<void()> func) {
        tests_.push_back({name, func});
    }
    
    void runAll() {
        std::cout << "\n========================================\n";
        std::cout << "Running " << tests_.size() << " tests...\n";
        std::cout << "========================================\n\n";
        
        for (const auto& test : tests_) {
            currentTest_ = test.name;
            std::cout << "[ RUN      ] " << test.name << "\n";
            
            try {
                test.func();
                std::cout << "[       OK ] " << test.name << "\n";
                passed_++;
            } catch (const std::exception& e) {
                std::cout << "[  FAILED  ] " << test.name << "\n";
                std::cout << "             " << e.what() << "\n";
                failed_++;
            }
        }
        
        std::cout << "\n========================================\n";
        std::cout << "Results: " << passed_ << " passed, " << failed_ << " failed\n";
        std::cout << "========================================\n";
    }
    
    int getFailedCount() const { return failed_; }
};

// Assertion macros
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        std::ostringstream oss; \
        oss << "ASSERT_TRUE failed: " << #condition << " at " << __FILE__ << ":" << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        std::ostringstream oss; \
        oss << "ASSERT_FALSE failed: " << #condition << " at " << __FILE__ << ":" << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        std::ostringstream oss; \
        oss << "ASSERT_EQ failed: expected " << (expected) << " but got " << (actual) \
            << " at " << __FILE__ << ":" << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_NE(val1, val2) \
    if ((val1) == (val2)) { \
        std::ostringstream oss; \
        oss << "ASSERT_NE failed: values are equal at " << __FILE__ << ":" << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_DOUBLE_EQ(expected, actual) \
    if (std::abs((expected) - (actual)) > 1e-9) { \
        std::ostringstream oss; \
        oss << "ASSERT_DOUBLE_EQ failed: expected " << (expected) << " but got " << (actual) \
            << " at " << __FILE__ << ":" << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_THROW(statement, exception_type) \
    { \
        bool caught = false; \
        try { statement; } \
        catch (const exception_type&) { caught = true; } \
        catch (...) { } \
        if (!caught) { \
            std::ostringstream oss; \
            oss << "ASSERT_THROW failed: expected " << #exception_type << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    }

#define ASSERT_NO_THROW(statement) \
    try { statement; } \
    catch (const std::exception& e) { \
        std::ostringstream oss; \
        oss << "ASSERT_NO_THROW failed: " << e.what() << " at " << __FILE__ << ":" << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define TEST(test_name) \
    void test_##test_name(); \
    struct TestRegistrar_##test_name { \
        TestRegistrar_##test_name() { \
            test::TestFramework::instance().addTest(#test_name, test_##test_name); \
        } \
    } testRegistrar_##test_name; \
    void test_##test_name()

} // namespace test

#endif // TEST_FRAMEWORK_H_
