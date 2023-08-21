#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

#include "type/Type.h"

#include <vipir/Module.h>

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

    std::ifstream file = std::ifstream(argv[1]);

    std::stringstream buffer;
    buffer << file.rdbuf();

    Type::Init();

    lexing::Lexer lexer(buffer.str());

    std::vector<lexing::Token> tokens = lexer.lex();

    Environment globalScope;

    parsing::Parser parser(tokens, &globalScope);
    
    vipir::Builder builder;
    vipir::Module module(argv[1]);

    for (auto& node : parser.parse())
    {
        node->emit(builder, module, &globalScope);
    }

    module.print(std::cout);

    module.optimize(vipir::OptimizationLevel::High);

    using namespace std::literals;
    std::ofstream outfile = std::ofstream(argv[1] + ".o"s);
    module.emit(outfile);

    return 0;
}