#ifndef VIPER_COMPILER_HXX
#define VIPER_COMPILER_HXX
#include <lexing/lexer.hxx>
#include <llvm/IR/LLVMContext.h>
#include <parsing/parser.hxx>
#include <fstream>
#include <map>

namespace Viper
{
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

        OutputType getOutputType() const;
        std::string_view getInputFileName() const;
        std::string_view getFileContents() const;
    private:
        std::unique_ptr<Lexing::Lexer> _lexer;
        std::unique_ptr<Parsing::Parser> _parser;

        llvm::LLVMContext _context;
        llvm::IRBuilder<> _builder;
        llvm::Module      _module;

        OutputType _outputType;
        std::string _inputFileName;
        std::optional<std::string> _outputFileName;
        std::string _contents;

        std::string _targetTriple;

        std::ifstream _handle;
    };
}

#endif