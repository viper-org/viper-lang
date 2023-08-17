#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

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

    lexing::Lexer lexer(buffer.str());

    std::vector<lexing::Token> tokens = lexer.lex();

    parsing::Parser parser(tokens);
    
    vipir::Builder builder;
    vipir::Module module(argv[1]);

    for (auto& node : parser.parse())
    {
        node->emit(builder, module);
    }

    using namespace std::literals;
    std::ofstream outfile = std::ofstream(argv[1] + ".o"s);
    module.emit(outfile);

    return 0;
}