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

        IntegerLiteral, StringLiteral,

        LeftParen, RightParen,
        LeftBracket, RightBracket,
        LeftSquareBracket, RightSquareBracket,
        Semicolon, Colon,
        Comma,

        Equals,
        Plus, Minus,
        PlusEquals, MinusEquals,
        DoubleEquals, BangEquals,
        LessThan, GreaterThan,
        LessEqual, GreaterEqual,

        Asperand, Ampersand,
        Pipe,
        Caret,
        Tilde,
        Star,
        RightArrow,
        Dot,

        Type,
        FuncKeyword,
        ReturnKeyword,
        LetKeyword, GlobalKeyword,
        IfKeyword, ElseKeyword,
        WhileKeyword, ForKeyword,
        TrueKeyword, FalseKeyword,
        NullptrKeyword,
        StructKeyword,
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