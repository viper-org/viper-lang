#ifndef QUARK_COMPILER_HXX
#define QUARK_COMPILER_HXX
#include <lexing/lexer.hxx>
#include <parsing/parser.hxx>
#include <parsing/AST/topLevel.hxx>
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

        std::vector<std::unique_ptr<Parsing::ASTTopLevel>> Compile();

        QuarkOutputType getOutputType() const;
        std::string_view getInputFileName() const;
        std::string_view getFileContents() const;
    private:
        std::unique_ptr<Lexing::Lexer> _lexer;
        std::unique_ptr<Parsing::Parser> _parser;
        QuarkOutputType _outputType;
        std::string _inputFileName;
        std::string _contents;

        std::ifstream _handle;
    };
}

#endif