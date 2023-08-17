#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <parser/Parser.h>

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

    for (auto& func : parser.parse())
    {
        std::cout << static_cast<parsing::Function*>(func.get())->getName() << "\n";
    }

    return 0;
}