#include <compiler.hxx>
#include <diagnostics.hxx>
#include <iostream>
#include <llvm/IR/Module.h>
#include <sstream>

namespace Viper
{
    Compiler::Compiler(ViperOutputType outputType, const std::string inputFileName)
        :_builder(_context), _module("viper", _context), _outputType(outputType), _inputFileName(inputFileName), _handle(inputFileName)
    {
        if(!_handle.is_open())
            Diagnostics::FatalError("viper", inputFileName + ": No such file or directory");

        Diagnostics::setFileName(_inputFileName);

        std::stringstream buf;
        buf << _handle.rdbuf();
        _contents = buf.str();
    }

    std::vector<llvm::Value*> Compiler::Compile()
    {
        _lexer = std::make_unique<Lexing::Lexer>(_contents);
        
        _parser = std::make_unique<Parsing::Parser>(_lexer->Lex(), _contents);

        std::vector<llvm::Value*> values;
        for(std::unique_ptr<Parsing::ASTTopLevel>& node : _parser->Parse())
            values.push_back(node->Generate(_context, _builder, _module));
        return values;
    }

    ViperOutputType Compiler::getOutputType() const
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