#include "lexer/Lexer.h"
#include "lexer/Token.h"

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

    for (auto&& tok : lexer.lex())
    {
        std::cout << tok.toString() << "\n";
    }

    return 0;
}