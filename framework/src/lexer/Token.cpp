#include "lexer/Token.h"

#include <sstream>

namespace lexing
{
    Token::Token(const TokenType tokenType, const std::string& text)
        :mTokenType(tokenType), mText(text)
    {
    }

    Token::Token(const TokenType tokenType, std::string&& text)
        :mTokenType(tokenType), mText(std::move(text))
    {
    }

    Token::Token(const TokenType tokenType)
        :mTokenType(tokenType), mText("")
    {
    }

    TokenType Token::getTokenType() const
    {
        return mTokenType;
    }

    const std::string& Token::getText() const
    {
        return mText;
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
        }
    }

    std::string Token::toString() const
    {
        std::stringstream ret;
        ret << TypeToString(mTokenType) << "(" << mText << ")";
        return ret.str();
    }

    bool Token::operator==(Token other)
    {
        return ((mText == other.mText) && (mTokenType == other.mTokenType));
    }
}