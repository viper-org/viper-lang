// Copyright 2024 solar-mist

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "type/Type.h"

#include <unordered_map>

namespace lexing
{
    Lexer::Lexer(const std::string& text)
        :mText(text), mPosition(0)
    {
    }

    const std::unordered_map<std::string_view, TokenType> keywords = {
        { "func",   TokenType::FuncKeyword },
        { "return", TokenType::ReturnKeyword },
        { "let",    TokenType::LetKeyword },
        { "if",     TokenType::IfKeyword },
        { "else",   TokenType::ElseKeyword },
        { "true",   TokenType::TrueKeyword },
        { "false",  TokenType::FalseKeyword },
    };

    std::vector<Token> Lexer::lex()
    {
        std::vector<Token> tokens;

        while (mPosition < mText.length())
        {
            std::optional<Token> token = nextToken();
            if(token.has_value())
                tokens.push_back(*token);
            consume();
        }

        return tokens;
    }

    char Lexer::current()
    {
        return mText[mPosition];
    }

    char Lexer::consume()
    {
        return mText[mPosition++];
    }

    char Lexer::peek(int offset)
    {
        return mText[mPosition + offset];
    }

    std::optional<Token> Lexer::nextToken()
    {
        if (std::isalpha(current()) || current() == '_') // Identifier
        {
            std::string text = std::string(1, current());

            while (std::isalnum(peek(1)) || peek(1) == '_')
            {
                consume();
                text += current();
            }

            if (keywords.find(text) != keywords.end())
            {
                return Token(keywords.at(text));
            }

            if (Type::Exists(text)) // TODO: Lookup type properly
            {
                return Token(TokenType::Type, std::move(text));
            }

            return Token(TokenType::Identifier, std::move(text));
        }

        if (std::isdigit(current()))
        {
            std::string text = std::string(1, current());

            while (std::isdigit(peek(1)))
            {
                consume();
                text += current();
            }

            return Token(TokenType::IntegerLiteral, std::move(text));
        }
        
        if (std::isspace(current())) // Newline, tab, space etc
        {
            return std::nullopt;
        }

        switch(current())
        {
            case '(':
                return Token(TokenType::LeftParen);
            case ')':
                return Token(TokenType::RightParen);

            case '{':
                return Token(TokenType::LeftBracket);
            case '}':
                return Token(TokenType::RightBracket);

            case ';':
                return Token(TokenType::Semicolon);
            case ':':
                return Token(TokenType::Colon);

            case '@':
                return Token(TokenType::Asperand);

            case '=':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::DoubleEquals);
                }
                return Token(TokenType::Equals);
            
            case '+':
                return Token(TokenType::Plus);
            case '-':
                if (peek(1) == '>')
                {
                    consume();
                    return Token(TokenType::RightArrow);
                }
                return Token(TokenType::Minus);
        }

        return Token(TokenType::Error, std::string(1, current())); // Unknown character
    }
}