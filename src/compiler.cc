#include "codegen/value/instruction/alloca.hh"
#include <compiler.hh>
#include <diagnostics.hh>
#include <lexing/lexer.hh>
#include <parsing/parser.hh>
#include <globals.hh>
#include <sstream>
#include <iostream>

std::map<std::string, Codegen::AllocaInst*> namedValues;

Compiler::Compiler(OutputType outputType, const std::string& inputFileName)
    :_outputType(outputType), _module(inputFileName), _builder(_module)
{
    _inputHandle = std::ifstream(inputFileName);
    if(!_inputHandle.is_open())
        Diagnostics::FatalError("viper", inputFileName + ": No such file or directory");
    Diagnostics::setFileName(inputFileName);
    
    std::stringstream buf;
    buf << _inputHandle.rdbuf();
    _contents = buf.str();

    _inputHandle.close();
}

void Compiler::Compile()
{
    Lexing::Lexer lexer(_contents);
    Parsing::Parser parser(lexer.Lex(), _contents);

    for(std::unique_ptr<ASTTopLevel>& node : parser.Parse())
        node->Generate(_module, _builder);
    
    std::cout << _module.Generate() << std::endl;
    for(std::pair<std::string, Codegen::AllocaInst*> alloca : namedValues)
        delete alloca.second;
}