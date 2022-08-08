#ifndef SKETCH_TOKEN_HXX
#define SKETCH_TOKEN_HXX
#include <string>

namespace Sketch
{
    namespace Lexing
    {
        enum class TokenType
        {
            LeftParen, RightParen,
            LeftBracket, RightBracket,

            Plus, Minus, Star, Slash,

            Integer,

            Asperand, RightArrow,

            Semicolon,

            EndOfFile
        };

        class Token
        {
        public:
            Token(TokenType type, const std::string text, unsigned int lineNumber);

            TokenType getType() const;
            std::string_view getText() const;
            unsigned int getLineNumber() const;

            friend std::ostream& operator<<(std::ostream& stream, Token token);
        private:
            TokenType _type;
            std::string _text;
            unsigned int _lineNumber;

            std::string typeAsString() const;
        };
    }
}

#endif