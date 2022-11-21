#ifndef VIPER_LEXER_HH
#define VIPER_LEXER_HH
#include <lexing/token.hh>
#include <optional>
#include <string>
#include <vector>

namespace Lexing
{
    class Lexer
    {
    public:
        Lexer(const std::string& text);

        std::vector<Lexing::Token> Lex();
    private:
        std::string _text;
        unsigned int _position;
        unsigned int _lineNumber;
        unsigned int _colNumber;
        char* _lineBegin;

        char Current() const;
        char Consume();
        char Peek(const int offset) const;

        std::optional<Lexing::Token> NextToken();
    };
}

#endif