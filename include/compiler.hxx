#ifndef QUARK_COMPILER_HXX
#define QUARK_COMPILER_HXX
#include <string>
#include <fstream>

namespace Quark
{
    enum class QuarkOutputType
    {
        LLVM,
        Assembly,
        Object,
    };

    class Compiler
    {
    public:
        Compiler(QuarkOutputType outputType, std::string inputFileName = "a.out");

        QuarkOutputType getOutputType() const;
        std::string_view getInputFileName() const;
    private:
        QuarkOutputType _outputType;
        std::string _inputFileName;

        std::ifstream handle;
    };
}

#endif