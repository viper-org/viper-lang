#ifndef VIPER_FRAMEWORK_LEXER_TOKEN_H
#define VIPER_FRAMEWORK_LEXER_TOKEN_H

#include <string>

namespace lexing
{
    enum class TokenType : int
    {
        Error,

        Identifier,

        IntegerLiteral,

        LeftParen, RightParen,
        LeftBracket, RightBracket,
        Semicolon,

        Asperand,
        RightArrow,

        Type,
        FuncKeyword,
        ReturnKeyword,
    };

    class Token
    {
    public:
        Token(const TokenType tokenType, const std::string& text);
        Token(const TokenType tokenType, std::string&& text);
        Token(const TokenType tokenType);

        TokenType getTokenType() const;
        const std::string& getText() const;

        std::string toString() const;

        bool operator==(Token other);

    private:
        TokenType mTokenType{ TokenType::Error };

        std::string mText;
    };
}

#endif