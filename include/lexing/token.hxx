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
            LeftSquareBracket, RightSquareBracket,

            Plus, Minus, Star, Slash,

            Increment, Decrement,

            Integer,
            Character,
            String,

            Identifier,

            Return,
            If,
            While,
            For,
            Break,
            
            Type,

            Extern,

            Struct,

            Equals,

            DoubleEquals,
            BangEquals,
            Bang,

            LessThan, GreaterThan,
            LessEquals, GreaterEquals,

            Ampersand, DoubleAmpersand,
            Pipe, DoublePipe,

            Asperand, RightArrow,

            Semicolon,
            Comma,
            Dot,

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