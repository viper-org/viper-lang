#include "type/types.hh"
#include <compiler.hh>
#include <lexing/lexer.hh>
#include <llvm/Support/raw_ostream.h>
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
    llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder = llvm::IRBuilder(ctx);
    llvm::Module mod(_inputFileName, ctx);

    InitBuiltinTypes(ctx);

    Lexing::Lexer* lexer = new Lexing::Lexer(_contents);
    Parsing::Parser* parser = new Parsing::Parser(lexer->Lex(), _contents);

    for(std::unique_ptr<Parsing::ASTNode>& node : parser->Parse())
        node->Emit(ctx, mod, builder, nullptr);
    delete parser;
    delete lexer;

    llvm::outs() << mod << "\n";
}

std::unordered_map<std::string, llvm::AllocaInst*> namedValues;