#include <Test.h>

#include <chrono>

struct failedTest
{
    std::string name;
    std::string suite;

    std::string file;
    int line;
    int col;
    
    std::string expansion;
};

std::vector<TestCase> tests;
std::vector<failedTest> failedTests;
size_t totalAssertions;
size_t failedAssertions;
TestCase* currentTest;

void testFailed(AssertInfo info)
{
    ++failedAssertions;

    failedTest fail;
    fail.file = info.sourceLocation.file_name();
    fail.line = info.sourceLocation.line();
    fail.col = info.sourceLocation.column();

    fail.expansion = info.lhs;
    switch (info.op)
    {
        case AssertOperator::EQ:
            fail.expansion += " == ";
            break;
        case AssertOperator::NEQ:
            fail.expansion += " != ";
            break;
        case AssertOperator::LE:
            fail.expansion += " <= ";
            break;
        case AssertOperator::LT:
            fail.expansion += " < ";
            break;
        case AssertOperator::GE:
            fail.expansion += " >= ";
            break;
        case AssertOperator::GT:
            fail.expansion += " > ";
            break;
        default:
            break;
    }
    fail.expansion += info.rhs;

    fail.name = currentTest->name;
    fail.suite = currentTest->suite;

    failedTests.push_back(fail);
}

void diagnostics()
{
    std::cout << "\n" << totalAssertions << " assertions. \x1b[;32m" << totalAssertions - failedAssertions << " passed, \x1b[;31m" << failedAssertions << " failed.\x1b[;0m\n";
    std::cout << tests.size() << " tests run. \x1b[;32m" << tests.size() - failedTests.size() << " succeeded, \x1b[;31m" << failedTests.size() << " failed.\x1b[;0m\n\n";
    for (failedTest& test : failedTests)
    {
        std::cout << "\x1b[;31mTest " << test.suite << "::" << test.name << "(" << test.file << ":" << test.line << ":" << test.col << ") failed with expansion:\x1b[;0m\n\t" << test.expansion << "\n";
    }
    if (failedTests.empty())
    {
        std::cout << "\x1b[;32mAll tests passed.\x1b[;0m\n";
    }
    else
    {
        std::exit(1);
    }
}

void runTests()
{
    for (TestCase& test : tests)
    {
        currentTest = &test;
        const auto start = std::chrono::system_clock::now().time_since_epoch();
        test.method();
        const auto end = std::chrono::system_clock::now().time_since_epoch();

        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << test.suite << "::" << test.name << "|duration|" << duration << "\n";
    }
    diagnostics();
}