#ifndef VIPER_COMPILER_HH
#define VIPER_COMPILER_HH
#include <optional>
#include <string>
#include <fstream>
#include <vector>

enum class OutputType
{
    LLVM,
    Assembly,
    Object,

    Library,
};

class Compiler
{
public:
    Compiler(OutputType outputType, const std::string& inputFileName, const std::optional<std::string>& outputFileName);

    std::string Compile();

    static void CompileLibrary(const std::vector<std::string>& objects, const std::stringstream& symbols, std::string_view output);

private:
    OutputType _outputType;

    std::ifstream _inputHandle;
    std::string _inputFileName;
    std::string _outputFileName;
    std::string _contents;
};

#endif