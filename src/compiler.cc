#include "ssa/value/value.hh"
#include <compiler.hh>
#include <lexing/lexer.hh>
#include <parsing/parser.hh>
#include <codegen/assembly.hh>
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
    _contents = buf.str();

    _inputHandle.close();
}

void Compiler::Compile()
{
    Lexing::Lexer lexer(_contents);
    Parsing::Parser parser(lexer.Lex(), _contents);
    SSA::Module module(_inputFileName);
    SSA::Builder builder(module);
    Codegen::Assembly assembly;

    for(std::unique_ptr<Parsing::ASTNode>& node : parser.Parse())
    {
        SSA::Value* value = node->Emit(builder);

        //value->Print(std::cout, 0);
        //std::cout << std::endl;
        value->Emit(assembly);

        value->Dispose();

    }
    assembly.Emit(std::cout);
}