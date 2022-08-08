#ifndef QUARK_TOKEN_HXX
#define QUARK_TOKEN_HXX
#include <string>

namespace Quark
{
    namespace Lexing
    {
        enum class TokenType
        {
            LeftParen, RightParen,
            LeftBracket, RightBracket,

            Plus, Minus, Star, Slash,

            Identifier,

            Integer,

            Asperand, RightArrow,

            Semicolon,

            EndOfFile, BadToken
        };

        class Token
        {
        public:
            Token(TokenType type, const std::string text, const unsigned int lineNumber, const unsigned int colNumber);

            TokenType        getType()       const;
            std::string_view getText()       const;
            unsigned int     getLineNumber() const;
            unsigned int     getColNumber()  const;

            friend std::ostream& operator<<(std::ostream& stream, Token token);
        private:
            TokenType _type;
            std::string _text;
            unsigned int _lineNumber;
            unsigned int _colNumber;

            std::string typeAsString() const;
        };
    }
}

#endif