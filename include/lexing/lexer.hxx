#ifndef QUARK_LEXER_HXX
#define QUARK_LEXER_HXX
#include <lexing/token.hxx>
#include <optional>
#include <string_view>
#include <vector>

namespace Quark
{
    namespace Lexing
    {
        class Lexer
        {
        public:
            Lexer(std::string text);

            std::vector<Token> Lex();
        private:
            std::string _text;
            unsigned int _position;
            unsigned int _lineNumber;
            unsigned int _colNumber;

            char Current() const;
            char Consume();
            char Peek(const int offset) const;

            std::optional<Token> NextToken();
        };
    }
}

#endif