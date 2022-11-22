#ifndef VIPER_COMPILER_HH
#define VIPER_COMPILER_HH
#include <optional>
#include <string>
#include <fstream>

enum class OutputType
{
    LLVM,
    Assembly,
    Object,
};

class Compiler
{
public:
    Compiler(OutputType outputType, const std::string& inputFileName, const std::optional<std::string>& outputFileName);

    void Compile();

private:
    OutputType _outputType;

    std::ifstream _inputHandle;
    std::string _inputFileName;
    std::string _outputFileName;
    std::string _contents;
};

#endif