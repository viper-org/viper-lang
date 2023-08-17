#ifndef VIPER_FRAMEWORK_LEXER_LEXER_H
#define VIPER_FRAMEWORK_LEXER_LEXER_H

#include <optional>
#include <string>
#include <vector>

namespace lexing
{
    class Token;

    class Lexer
    {
    public:
        Lexer(const std::string& text);

        std::vector<Token> lex();
    private:
        std::string mText;

        int mPosition{ 0 };

        char current();
        char consume();
        char peek(int offset);

        std::optional<Token> nextToken();
    };
}

#endif