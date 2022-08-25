#include <compiler.hh>
#include <diagnostics.hh>
#include <lexing/lexer.hh>
#include <parsing/parser.hh>
#include <sstream>
#include <iostream>

Compiler::Compiler(OutputType outputType, const std::string& inputFileName)
    :_outputType(outputType), _inputFileName(inputFileName)
{
    _inputHandle = std::ifstream(inputFileName);
    if(!_inputHandle.is_open())
        Diagnostics::FatalError("viper", inputFileName + ": No such file or directory");
    Diagnostics::setFileName(_inputFileName);
    
    std::stringstream buf;
    buf << _inputHandle.rdbuf();
    _contents = buf.str();

    _inputHandle.close();
}

void Compiler::Compile()
{
    Lexer lexer(_contents);
    Parser parser(lexer.Lex(), _contents);

    for(std::unique_ptr<ASTTopLevel>& node : parser.Parse())
    {
        node->Print(std::cout);
    }
}