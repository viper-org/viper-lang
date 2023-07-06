#include <Test.h>


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

void diagnostics(OutputType outputType)
{
    if(outputType == OutputType::Markdown)
    {
        std::cout << "# Test Results";
        std::cout << "\n" << tests.size() << " tests run. :heavy_check_mark:" << tests.size() - failedTests.size() << " succeeded, :x:" << failedTests.size() << "failed.\n\n";
        for (failedTest& test : failedTests)
        {
            std::cout << ":x:Test " << test.suite << "::" << test.name << "(" << test.file << ":" << test.line << ":" << test.col << ") failed with expansion:\n\t" << test.condition << "\n";
        }
        if (failedTests.empty())
        {
            std::cout << ":heavy_check_mark:All tests passed.\n";
        }
        else
        {
            std::exit(1);
        }
    }
    else if(outputType == OutputType::Text)
    {
        std::cout << "\n" << tests.size() << " tests run. \x1b[;32m" << tests.size() - failedTests.size() << " succeeded, \x1b[;31m" << failedTests.size() << "failed.\x1b[;0m\n\n";
        for (failedTest& test : failedTests)
        {
            std::cout << "\x1b[;31mTest " << test.suite << "::" << test.name << "(" << test.file << ":" << test.line << ":" << test.col << ") failed with expansion:\x1b[;0m\n\t" << test.condition << "\n";
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
}

void runTests(OutputType outputType)
{
    for (TestCase& test : tests)
    {
        if (outputType != OutputType::Markdown)
        {
            std::cout << "Running test " << test.suite << "::" << test.name << "\n";
        }
        test.method();
    }
    diagnostics(outputType);
}