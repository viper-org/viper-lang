#ifndef QUARK_COMPILER_HXX
#define QUARK_COMPILER_HXX
#include <lexing/lexer.hxx>
#include <string>
#include <fstream>
#include <memory>

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
        Compiler(QuarkOutputType outputType, const std::string inputFileName = "a.out");

        std::vector<Lexing::Token> Compile();

        QuarkOutputType getOutputType() const;
        std::string_view getInputFileName() const;
    private:
        std::unique_ptr<Lexing::Lexer> _lexer;
        QuarkOutputType _outputType;
        std::string _inputFileName;

        std::ifstream handle;
    };
}

#endif