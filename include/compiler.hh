#ifndef VIPER_COMPILER_HH
#define VIPER_COMPILER_HH
#include <type/types.hh>
#include <optional>
#include <string>
#include <fstream>
#include <vector>
#include <memory>

enum class OutputType
{
    LLVM,
    Assembly,
    Object,

    Library,
    Executable,
};

class Compiler
{
public:
    Compiler(OutputType outputType, const std::string& inputFileName, const std::optional<std::string>& outputFileName, const std::vector<std::string>& libraries);

    std::string GetSymbols();
    std::pair<std::string, std::string> Compile(std::string symbols);

    static void CompileLibrary(const std::vector<std::string>& objects, const std::stringstream& symbols, std::string_view output);
    static void CompileExecutable(std::vector<std::string>& objectFiles, const std::vector<std::string>& libraries, const std::string& outputFileName);

private:
    OutputType _outputType;

    std::ifstream _inputHandle;
    std::string _inputFileName;
    std::optional<std::string> _outputFileName;
    std::string _contents;
    std::vector<std::string> _libraries;
};

#endif