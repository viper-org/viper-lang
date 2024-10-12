#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <diagnostic/Diagnostic.h>

#include <parser/Parser.h>

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
    std::string text = ss.str();

    diagnostic::Diagnostics diag;
    diag.setText(text);

    lexer::Lexer lexer(text, argv[1]);
    auto tokens = lexer.lex();
    lexer.scanInvalidTokens(tokens, diag);

    Scope globalScope(nullptr, "", true);
    parser::Parser parser(tokens, diag, &globalScope);
    auto ast = parser.parse();

    return 0;
}