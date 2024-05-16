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
        DoubleLeftSquareBracket, DoubleRightSquareBracket,
        Semicolon, Colon, DoubleColon,
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
        BreakKeyword, ContinueKeyword,
        TrueKeyword, FalseKeyword,
        NullptrKeyword,
        StructKeyword, PrivateKeyword,
        ImportKeyword,
        NamespaceKeyword, ExportKeyword,
        UsingKeyword,
        SizeofKeyword,
    };

    struct SourceLocation
    {
        int column;
        int line;

        int position; // actual position into the text
    };

    class Token
    {
    public:
        Token(const TokenType tokenType, std::string text, SourceLocation start, SourceLocation end);
        Token(const TokenType tokenType, SourceLocation start, SourceLocation end);

        TokenType getTokenType() const;
        std::string getId() const;
        const std::string& getText() const;

        SourceLocation getStart();
        SourceLocation getEnd();

        std::string toString() const;

        bool operator==(Token other);

    private:
        TokenType mTokenType{ TokenType::Error };

        std::string mText;

        SourceLocation mStart;
        SourceLocation mEnd;
    };
}

#endif