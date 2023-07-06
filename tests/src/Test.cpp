#include <Test.h>

#include <format>


struct failedTest
{
    std::string name;
    std::string suite;

    std::string file;
    int line;
    int col;
    
    std::string condition;
};

std::vector<TestCase> tests;
std::vector<failedTest> failedTests;

void testFailed(std::source_location sourceLocation, const char* condition)
{
    failedTest fail;
    fail.file = sourceLocation.file_name();
    fail.line = sourceLocation.line();
    fail.col = sourceLocation.column();
    
    fail.condition = condition;

    std::string funcName = sourceLocation.function_name();

    size_t firstDollar = funcName.find_first_of('$') + 1;

    std::string testName = funcName.substr(firstDollar, funcName.find_last_of('$') - firstDollar);
    std::string suiteName = funcName.substr(funcName.find_last_of('$') + 1, funcName.find_first_of('(') - funcName.find_last_of('$') - 1);

    fail.name = testName;
    fail.suite = suiteName;

    failedTests.push_back(fail);
}

void diagnostics()
{
    std::cout << std::format("\n{} tests run. \x1b[;32m{} succeeded, \x1b[;31m{} failed.\x1b[;0m\n\n", tests.size(), tests.size() - failedTests.size(), failedTests.size());
    for (failedTest& test : failedTests)
    {
        std::cout << std::format("\x1b[;31mTest {}::{}({}:{}:{}) failed with expansion:\x1b[;0m\n\t{}\n", test.suite, test.name, test.file, test.line, test.col, test.condition);
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
        std::cout << std::format("Running test {}::{}\n", test.suite, test.name);
        test.method();
    }
    diagnostics();
}