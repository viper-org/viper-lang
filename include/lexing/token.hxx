#ifndef VIPER_TOKEN_HXX
#define VIPER_TOKEN_HXX
#include <string>

namespace Viper
{
    namespace Lexing
    {
        enum class TokenType
        {
            LeftParen, RightParen,
            LeftBracket, RightBracket,

            Plus, Minus, Star, Slash,

            Integer,

            Identifier,

            Return,
            If,
            While,
            Break,
            Type,

            Equals,

            DoubleEquals,
            BangEquals,
            Bang,

            Ampersand, DoubleAmpersand,
            Pipe, DoublePipe,

            Asperand, RightArrow,

            Semicolon,

            EndOfFile, BadToken
        };

        class Token
        {
        public:
            Token(TokenType type,
            const unsigned int start, const unsigned int end,
            const unsigned int lineNumber, const unsigned int colNumber);

            TokenType    getType()       const;
            unsigned int getStart()      const;
            unsigned int getEnd()        const;
            unsigned int getLineNumber() const;
            unsigned int getColNumber()  const;

            std::string typeAsString() const;

            friend std::ostream& operator<<(std::ostream& stream, Token token);
        private:
            TokenType _type;
            unsigned int _start;
            unsigned int _end;

            unsigned int _lineNumber;
            unsigned int _colNumber;
        };
    }
}

#endif