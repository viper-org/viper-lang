#include <compiler.hxx>
#include <diagnostics.hxx>
#include <iostream>
#include <sstream>

namespace Quark
{
    Compiler::Compiler(QuarkOutputType outputType, const std::string inputFileName)
        :_outputType(outputType), _inputFileName(inputFileName), _handle(inputFileName)
    {
        if(!_handle.is_open())
            Diagnostics::FatalError("qrk", inputFileName + ": No such file or directory");

        Diagnostics::setFileName(_inputFileName);

        std::stringstream buf;
        buf << _handle.rdbuf();
        _contents = buf.str();
    }

    std::vector<std::unique_ptr<Parsing::ASTTopLevel>> Compiler::Compile()
    {
        _lexer = std::make_unique<Lexing::Lexer>(_contents);
        
        _parser = std::make_unique<Parsing::Parser>(_lexer->Lex(), _contents);

        return _parser->Parse();
    }

    QuarkOutputType Compiler::getOutputType() const
    {
        return _outputType;
    }

    std::string_view Compiler::getInputFileName() const
    {
        return _inputFileName;
    }

    std::string_view Compiler::getFileContents() const
    {
        return _contents;
    }
}