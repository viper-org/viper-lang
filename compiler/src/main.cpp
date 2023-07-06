#include <lexer/Lexer.h>
#include <lexer/Token.h>

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

    Lexing::Lexer lexer(buffer.str());

    std::vector<Lexing::Token> tokens = lexer.lex();

    for (const Lexing::Token& token : tokens)
    {
        std::cout << token.toString() << "\n";
    }

    return 0;
}