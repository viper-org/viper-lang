// Copyright 2024 solar-mist


#include "preprocessor/Preprocessor.h"

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

#include "type/Type.h"

#include <vipir/IR/IRBuilder.h>
#include <vipir/Module.h>
#include <vipir/ABI/SysV.h>

#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "viper: no input files";
        std::exit(1);
    }

    std::string inputFilePath;
    std::string outputFilePath;
    bool outputIR = false;

    preprocessor::Preprocessor preprocessor;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg.starts_with('-'))
        {
            switch(arg[1])
            {
                case 'I':
                    if (arg.length() == 2)
                        preprocessor.addIncludePath(argv[++i]);
                    else
                        preprocessor.addIncludePath(arg.substr(2));
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

                default: // TODO: Proper error
                    std::cerr << "Unrecognized command-line option: " << arg << "\n";
                    std::exit(1);
            }
        }
        else
        {
            inputFilePath = arg;
        }
    }

    if (outputFilePath.empty())
    {
        outputFilePath = inputFilePath + (outputIR ? ".i" : ".o");
    }

    std::ifstream file = std::ifstream(inputFilePath);

    std::stringstream buffer;
    buffer << file.rdbuf();

    Type::Init();

    preprocessor.addText(buffer.str());
    preprocessor.preprocess();

    lexing::Lexer lexer(preprocessor.getText());

    std::vector<lexing::Token> tokens = lexer.lex();

    parser::Parser parser(tokens);
    
    vipir::IRBuilder builder;
    vipir::Module module(inputFilePath);
    module.setABI<vipir::abi::SysV>();

    for (auto& node : parser.parse())
    {
        node->emit(builder, module, nullptr);
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