#ifndef VIPER_COMPILER_HXX
#define VIPER_COMPILER_HXX
#include <lexing/lexer.hxx>
#include <parsing/parser.hxx>
#include <parsing/AST/topLevel.hxx>
#include <string>
#include <fstream>
#include <memory>

namespace Viper
{
    enum class ViperOutputType
    {
        LLVM,
        Assembly,
        Object,
    };

    class Compiler
    {
    public:
        Compiler(ViperOutputType outputType, const std::string inputFileName = "a.out");

        std::vector<std::unique_ptr<Parsing::ASTTopLevel>> Compile();

        ViperOutputType getOutputType() const;
        std::string_view getInputFileName() const;
        std::string_view getFileContents() const;
    private:
        std::unique_ptr<Lexing::Lexer> _lexer;
        std::unique_ptr<Parsing::Parser> _parser;
        ViperOutputType _outputType;
        std::string _inputFileName;
        std::string _contents;

        std::ifstream _handle;
    };
}

#endif