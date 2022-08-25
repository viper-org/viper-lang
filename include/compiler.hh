#ifndef VIPER_COMPILER_HH
#define VIPER_COMPILER_HH
#include <string>
#include <fstream>

enum class OutputType
{
    Assembly,
    // TODO: Add more output types
};

class Compiler
{
public:
    Compiler(OutputType outputType, const std::string& inputFileName);

    void Compile();

private:
    OutputType _outputType;
    std::string _inputFileName;

    std::ifstream _inputHandle;
    std::string _contents;
};

#endif