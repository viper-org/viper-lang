#include <compiler.hh>
#include <lexing/lexer.hh>
#include <parsing/parser.hh>
#include <diagnostics.hh>
#include <sstream>
#include <iostream>


Compiler::Compiler(OutputType outputType, const std::string& inputFileName)
    :_outputType(outputType), _inputFileName(inputFileName)
{
    _inputHandle = std::ifstream(inputFileName);
    if(!_inputHandle.is_open())
        Diagnostics::FatalError("viper", inputFileName + ": No such file or directory");
    Diagnostics::setFileName(inputFileName);
    
    std::stringstream buf;
    buf << _inputHandle.rdbuf();
    _contents = buf.str() + '\n';

    _inputHandle.close();
}

void Compiler::Compile()
{
    Lexing::Lexer* lexer = new Lexing::Lexer(_contents);
    Parsing::Parser* parser = new Parsing::Parser(lexer->Lex(), _contents);
    for(std::unique_ptr<Parsing::ASTNode>& node : parser->Parse())
    {
        node->Print(std::cout, 0);
        std::cout << '\n';
    }
    delete parser;
    delete lexer;
}