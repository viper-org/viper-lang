#ifndef VIPER_COMPILER_HXX
#define VIPER_COMPILER_HXX
#include <lexing/lexer.hxx>
#include <codegen/symbol.hxx>
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
        Executable,
        Library,
    };

    class Compiler
    {
    public:
        Compiler(OutputType outputType, const std::string& inputFileName, const std::optional<std::string>& outputFileName, const std::vector<std::string>& libraries);

        std::pair<std::vector<std::unique_ptr<CodeGen::Symbol>>, std::string> Compile();
        static void GenerateLibrary(const std::vector<std::string>& objectFiles, const std::vector<std::unique_ptr<Viper::CodeGen::Symbol>>& symbols, const std::string& outputFileName);
        static void GenerateExecutable(std::vector<std::string> objectFiles, const std::vector<std::string>& libraries, const std::string& outputFileName);

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
        std::vector<std::string> _libraries;
        std::string _contents;

        std::string _targetTriple;

        std::ifstream _handle;
    };
}

#endif