#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "viper: no input files\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "viper: could not find file '" << argv[1] << "'\n";
        return 1;
    }

    std::stringstream ss;
    ss << inputFile.rdbuf();

    lexer::Lexer lexer(ss.str(), argv[1]);
    auto tokens = lexer.lex();
    for (auto& token : tokens)
    {
        std::cout << token.getText() << "\n";
    }

    return 0;
}