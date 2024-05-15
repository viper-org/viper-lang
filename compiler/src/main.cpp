// Copyright 2024 solar-mist


#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

#include "type/Type.h"

#include "diagnostic/Diagnostic.h"

#include "symbol/Import.h"

#include <vipir/IR/IRBuilder.h>
#include <vipir/Module.h>
#include <vipir/ABI/SysV.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** argv)
{
    diagnostic::Diagnostics diag;
    diag.setErrorSender("viper");

    std::string inputFilePath;
    std::string outputFilePath;
    bool outputIR = false;
    bool optimize = false;

    symbol::ImportManager importManager;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg.starts_with('-'))
        {
            switch(arg[1])
            {
                case 'I':
                    if (arg.length() == 2)
                    {
                        importManager.addSearchPath(argv[++i]);
                    }
                    else
                    {
                        importManager.addSearchPath(arg.substr(2));
                    }
                    break;

                case 'i':
                    outputIR = true;
                    break;

                case 'o':
                    if (arg.length() == 2)
                        outputFilePath = argv[++i];
                    else
                        outputFilePath = arg.substr(2);
                    break;

                case 'O':
                    optimize = true;
                    break;

                default:
                    diag.fatalError(std::format("Unrecognized command-line option: {}", arg));
            }
        }
        else
        {
            inputFilePath = arg;
        }
    }

    if (inputFilePath.empty())
    {
        diag.fatalError("no input files");
    }
    if (!std::filesystem::exists(inputFilePath))
    {
        diag.fatalError(std::format("{}: no such file or directory", inputFilePath));
    }
    diag.setFileName(inputFilePath);

    if (outputFilePath.empty())
    {
        outputFilePath = inputFilePath + (outputIR ? ".i" : ".o");
    }

    std::ifstream file = std::ifstream(inputFilePath);

    std::stringstream buffer;
    buffer << file.rdbuf();

    Type::Init();

    diag.setText(buffer.str());
    lexing::Lexer lexer(buffer.str(), diag);

    std::vector<lexing::Token> tokens = lexer.lex();

    parser::Parser parser(tokens, diag, importManager);
    
    vipir::IRBuilder builder;
    vipir::Module module(inputFilePath);
    module.setABI<vipir::abi::SysV>();


    for (auto& node : parser.parse())
    {
        node->emit(builder, module, nullptr, diag);
    }

    if (optimize)
    {
        module.addPass(vipir::Pass::PeepholeOptimization);
    }
    
    std::ofstream outputFile = std::ofstream(outputFilePath);
    if (outputIR)
    {
        module.print(outputFile);
    }
    else
    {
        module.emit(outputFile, vipir::OutputFormat::ELF);
    }

    return 0;
}