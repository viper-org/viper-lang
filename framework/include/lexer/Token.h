// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_LEXER_TOKEN_H
#define VIPER_FRAMEWORK_LEXER_TOKEN_H 1

#include "lexer/SourceLocation.h"

#include <string>

namespace lexer
{
    enum class TokenType
    {
        // Keywords
        FuncKeyword,
        ReturnKeyword,
        I32Keyword, // TODO: Add proper types

        // Symbols
        Asperand,
        RightArrow,
        LeftParen,
        RightParen,
        LeftBrace,
        RightBrace,
        Semicolon,

        // Literals
        IntegerLiteral,

        // Misc
        Identifier,
        Error,
    };

    class Token
    {
    public:
        Token(std::string text, TokenType tokenType, SourceLocation startLocation, SourceLocation endLocation);

        std::string_view getText() const;
        TokenType getTokenType() const;

        SourceLocation getStartLocation() const;
        SourceLocation getEndLocation() const;

    private:
        std::string mText;
        TokenType mTokenType;

        SourceLocation mStartLoc;
        SourceLocation mEndLoc;
    };
}

#endif // VIPER_FRAMEWORK_LEXER_TOKEN_H