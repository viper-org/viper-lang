// Copyright 2024 solar-mist

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
        Semicolon, Colon,

        Equals,
        Plus, Minus,
        DoubleEquals, BangEquals,
        LessThan, GreaterThan,
        LessEqual, GreaterEqual,

        Asperand,
        RightArrow,

        Type,
        FuncKeyword,
        ReturnKeyword,
        LetKeyword,
        IfKeyword, ElseKeyword,
        WhileKeyword,
        TrueKeyword, FalseKeyword
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