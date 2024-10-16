// Copyright 2024 solar-mist

#ifndef VIPER_TESTS_COMPILER_TESTS_H
#define VIPER_TESTS_COMPILER_TESTS_H 1

#include "ViperAssert.h"

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
extern size_t totalAssertions;

void testFailed(AssertInfo info);
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

#define REQUIRE(condition) \
    do { \
        ++totalAssertions; \
        auto info = AssertDecomposer{AssertInfo{#condition, std::source_location::current(), AssertOperator::None, AssertResult::None}} < condition; \
        if (info.result != AssertResult::True)\
        { \
            testFailed(info); \
            return; \
        } \
    } while(0)

#define REQUIRE_FALSE(condition) \
    do { \
        ++totalAssertions; \
        auto info = AssertDecomposer{AssertInfo{#condition, std::source_location::current(), AssertOperator::None, AssertResult::None}} < condition; \
        if (info.result != AssertResult::False)\
        { \
            testFailed(info); \
            return; \
        } \
    } while(0)

#endif // VIPER_TESTS_COMPILER_TESTS_H