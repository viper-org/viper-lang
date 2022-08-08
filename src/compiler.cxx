#include <compiler.hxx>
#include <diagnostics.hxx>
#include <iostream>
#include <sstream>

namespace Quark
{
    Compiler::Compiler(QuarkOutputType outputType, const std::string inputFileName)
        :_outputType(outputType), _inputFileName(inputFileName), handle(inputFileName)
    {
        if(!handle.is_open())
            Diagnostics::FatalError("qrk", inputFileName + ": No such file or directory");
    }

    std::vector<Lexing::Token> Compiler::Compile()
    {
        std::stringstream buffer;
        buffer << handle.rdbuf();

        _lexer = std::make_unique<Lexing::Lexer>(buffer.str(), _inputFileName);
        return _lexer->Lex();
    }

    QuarkOutputType Compiler::getOutputType() const
    {
        return _outputType;
    }

    std::string_view Compiler::getInputFileName() const
    {
        return _inputFileName;
    }
}