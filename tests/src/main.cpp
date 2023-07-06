#include <Test.h>

#include <cstring>

int main(int argc, char** argv)
{
    if (argc < 2)
        runTests(OutputType::Text);
    else
    {
        if (!std::strcmp(argv[1], "markdown"))
        {
            runTests(OutputType::Markdown);
        }
        else if (!std::strcmp(argv[1], "text"))
        {
            runTests(OutputType::Text);
        }
        else
        {
            std::cout << "Unkown output type\n";
        }
    }
}