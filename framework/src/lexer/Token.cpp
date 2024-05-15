// Copyright 2024 solar-mist

#include "lexer/Token.h"

#include <sstream>
#include <format>

namespace lexing
{
    Token::Token(const TokenType tokenType, std::string text, SourceLocation start, SourceLocation end)
        : mTokenType(tokenType)
        , mText(std::move(text))
        , mStart(start)
        , mEnd(end)
    {
    }

    Token::Token(const TokenType tokenType, SourceLocation start, SourceLocation end)
        : mTokenType(tokenType)
        , mText("")
        , mStart(start)
        , mEnd(end)
    {
    }

    TokenType Token::getTokenType() const
    {
        return mTokenType;
    }

    std::string Token::getId() const
    {
        switch (mTokenType)
        {
            case TokenType::Identifier:
                return "identifier";
            case TokenType::IntegerLiteral:
                return "integer literal";
            case TokenType::StringLiteral:
                return "string literal";
            case TokenType::LeftParen:
                return "(";
            case TokenType::RightParen:
                return ")";
            case TokenType::LeftBracket:
                return "{";
            case TokenType::RightBracket:
                return "}";
            case TokenType::LeftSquareBracket:
                return "[";
            case TokenType::RightSquareBracket:
                return "]";
            case TokenType::DoubleLeftSquareBracket:
                return "[[";
            case TokenType::DoubleRightSquareBracket:
                return "]]";
            case TokenType::Semicolon:
                return ";";
            case TokenType::Colon:
                return ":";
            case TokenType::DoubleColon:
                return "::";
            case TokenType::Comma:
                return ",";
            case TokenType::Equals:
                return "=";
            case TokenType::Plus:
                return "+";
            case TokenType::Minus:
                return "-";
            case TokenType::PlusEquals:
                return "+=";
            case TokenType::MinusEquals:
                return "-=";
            case TokenType::DoubleEquals:
                return "==";
            case TokenType::BangEquals:
                return "!=";
            case TokenType::LessThan:
                return "<";
            case TokenType::GreaterThan:
                return ">";
            case TokenType::LessEqual:
                return "<=";
            case TokenType::GreaterEqual:
                return ">=";
            case TokenType::Asperand:
                return "@";
            case TokenType::Ampersand:
                return "&";
            case TokenType::Pipe:
                return "|";
            case TokenType::Caret:
                return "^";
            case TokenType::Tilde:
                return "~";
            case TokenType::Star:
                return "*";
            case TokenType::RightArrow:
                return "->";
            case TokenType::Dot:
                return ".";
            case TokenType::Type:
                return "type";
            case TokenType::FuncKeyword:
                return "func";
            case TokenType::ReturnKeyword:
                return "return";
            case TokenType::LetKeyword:
                return "let";
            case TokenType::GlobalKeyword:
                return "global";
            case TokenType::IfKeyword:
                return "if";
            case TokenType::ElseKeyword:
                return "else";
            case TokenType::WhileKeyword:
                return "while";
            case TokenType::ForKeyword:
                return "for";
            case TokenType::BreakKeyword:
                return "break";
            case TokenType::ContinueKeyword:
                return "continue";
            case TokenType::TrueKeyword:
                return "true";
            case TokenType::FalseKeyword:
                return "false";
            case TokenType::NullptrKeyword:
                return "nullptr";
            case TokenType::StructKeyword:
                return "struct";
            case TokenType::PrivateKeyword:
                return "private";
            case TokenType::ImportKeyword:
                return "import";
            case TokenType::NamespaceKeyword:
                return "namespace";
            case TokenType::ExportKeyword:
                return "export";
            case TokenType::UsingKeyword:
                return "using";
            case TokenType::Error:
                return mText;
        }
    }

    const std::string& Token::getText() const
    {
        return mText;
    }

    SourceLocation Token::getStart()
    {
        return mStart;
    }
    SourceLocation Token::getEnd()
    {
        return mEnd;
    }

    static inline const char* TypeToString(TokenType tokenType)
    {
        switch(tokenType)
        {
            case TokenType::Error:
                return "Error";

            case TokenType::Identifier:
                return "Identifier";

            case TokenType::LeftParen:
                return "LeftParen";
            case TokenType::RightParen:
                return "RightParen";
            case TokenType::LeftBracket:
                return "LeftBracket";
            case TokenType::RightBracket:
                return "RightBracket";

            case TokenType::IntegerLiteral:
                return "IntegerLiteral";
            
            case TokenType::Semicolon:
                return "Semicolon";
            case TokenType::Colon:
                return "Colon";
            case TokenType::Asperand:
                return "Asperand";
            case TokenType::RightArrow:
                return "RightArrow";

            case TokenType::Equals:
                return "Equals";
            case TokenType::Plus:
                return "Plus";
            case TokenType::Minus:
                return "Minus";
            case TokenType::PlusEquals:
                return "PlusEquals";
            case TokenType::MinusEquals:
                return "MinusEquals";

            case TokenType::DoubleEquals:
                return "DoubleEquals";
            case TokenType::BangEquals:
                return "BangEquals";
            case TokenType::LessThan:
                return "LessThan";
            case TokenType::GreaterThan:
                return "GreaterThan";
            case TokenType::LessEqual:
                return "LessEqual";
            case TokenType::GreaterEqual:
                return "GreaterEqual";

            case TokenType::Type:
                return "Type";
            case TokenType::FuncKeyword:
                return "Func";
            case TokenType::ReturnKeyword:
                return "Return";
            case TokenType::LetKeyword:
                return "Let";
            case TokenType::IfKeyword:
                return "If";
            case TokenType::ElseKeyword:
                return "Else";
            case TokenType::WhileKeyword:
                return "While";
            case TokenType::TrueKeyword:
                return "True";
            case TokenType::FalseKeyword:
                return "False";
        }
        return ""; // unreachable
    }

    std::string Token::toString() const
    {
        return std::format("{}({})", TypeToString(mTokenType), mText);
    }

    bool Token::operator==(Token other)
    {
        return ((mText == other.mText) && (mTokenType == other.mTokenType));
    }
}