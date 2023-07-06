#ifndef VIPER_TESTS_TESTS_H
#define VIPER_TESTS_TESTS_H 1

#include <vector>
#include <functional>
#include <iostream>
#include <source_location>

struct TestCase
{
    std::function<void()> method;
    std::string name;
    std::string suite;
};

extern std::vector<TestCase> tests;

void testFailed(std::source_location sourceLocation, const char* condition);
void diagnostics();

void runTests();

#define TEST(name, suite) \
    void test$##name##$##suite(); \
    struct test_##name##_##suite_registrar { \
        test_##name##_##suite_registrar() {\
            tests.push_back({test$##name##$##suite, #name, #suite}); \
        } \
    }; \
    volatile test_##name##_##suite_registrar test_##name##_##suite_registrar_object; \
    void test$##name##$##suite()

#define TEST_FAIL_TEST(expansion) \
    do { \
        testFailed(std::source_location::current(), #expansion); \
        return; \
    } while(0)

#define REQUIRE(condition) \
    do { \
        if (!(condition))\
        { \
            TEST_FAIL_TEST(condition); \
        } \
    } while(0)

#define REQUIRE_FALSE(condition) \
    do { \
        if ((condition))\
        { \
            TEST_FAIL_TEST(condition); \
        } \
    } while(0)

#endif