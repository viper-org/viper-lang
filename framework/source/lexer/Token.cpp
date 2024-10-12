// Copyright 2024 solar-mist

#include "lexer/Token.h"

namespace lexer
{
    Token::Token(std::string text, TokenType tokenType, SourceLocation start, SourceLocation end)
        : mText(std::move(text))
        , mTokenType(tokenType)
        , mStartLoc(start)
        , mEndLoc(end)
    {
    }

    std::string_view Token::getText() const
    {
        return mText;
    }

    std::string_view Token::getName() const
    {
        switch (mTokenType)
        {
            case TokenType::FuncKeyword:
                return "func";
            case TokenType::ReturnKeyword:
                return "return";
            case TokenType::I32Keyword:
                return "i32";
            case TokenType::RightArrow:
                return "->";
            case TokenType::LeftParen:
                return "(";
            case TokenType::RightParen:
                return ")";
            case TokenType::LeftBrace:
                return "{";
            case TokenType::RightBrace:
                return "}";
            case TokenType::Semicolon:
                return ";";
            case TokenType::IntegerLiteral:
                return "integer literal";
            case TokenType::Identifier:
                return "an identifier";
            case TokenType::Error:
                return "Error";
        }
        return "";
    }

    TokenType Token::getTokenType() const
    {
        return mTokenType;
    }


    SourceLocation Token::getStartLocation() const
    {
        return mStartLoc;
    }

    SourceLocation Token::getEndLocation() const
    {
        return mEndLoc;
    }
}