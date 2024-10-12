// Copyright 2024 solar-mist

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include <format>
#include <unordered_map>

namespace lexer
{
    Lexer::Lexer(std::string_view text, std::string_view fileName)
        : mText(text)
        , mFileName(fileName)
        , mSourceLocation({fileName, 1, 1, 0})
        , mPosition(0)
    {
    }

    static const std::unordered_map<std::string_view, TokenType> keywords = {
        { "func",   TokenType::FuncKeyword },
        { "return", TokenType::ReturnKeyword },
        { "i32",    TokenType::I32Keyword },
    };


    std::vector<Token> Lexer::lex()
    {
        std::vector<Token> ret;

        while (mPosition < mText.length())
        {
            auto token = getNextToken();
            if (token)
            {
                ret.push_back(token.value());
            }
            consume();
        }

        return ret;
    }
    
    void Lexer::scanInvalidTokens(std::vector<Token>& tokens, diagnostic::Diagnostics& diag)
    {
        bool foundInvalidTokens = false;
        for (auto& token : tokens)
        {
            if (token.getTokenType() == TokenType::Error)
            {
                foundInvalidTokens = true;
                diag.reportCompilerError(token.getStartLocation(),
                    token.getEndLocation(),
                    std::format("stray '{}{}{}' in program", fmt::bold, token.getText(), fmt::defaults));
            }
        }

        if (foundInvalidTokens)
        {
            std::exit(EXIT_FAILURE);
        }
    }


    char Lexer::current()
    {
        return mText[mPosition];
    }

    char Lexer::consume()
    {
        mSourceLocation.col += 1;
        if (mText[mPosition++] == '\n')
        {
            mSourceLocation.col = 0;
            mSourceLocation.line += 1;
        }
        mSourceLocation.position += 1;
        return current();
    }

    char Lexer::peek(int offset)
    {
        return mText[mPosition + offset];
    }

    std::optional<Token> Lexer::getNextToken()
    {
        SourceLocation start = mSourceLocation;

        if (isIdentifierStart(current()))
        {
            std::string text = std::string(1, current());
            while (isIdentifier(peek(1)))
            {
                consume();
                text += current();
            }

            if (keywords.find(text) != keywords.end())
            {
                return Token(std::move(text), keywords.at(text), start, mSourceLocation);
            }
            
            return Token(std::move(text), TokenType::Identifier, start, mSourceLocation);
        }

        if (std::isdigit(current()))
        {
            std::string text = std::string(1, current());
            if (current() == '0')
            {
                if (peek(1) == 'x') // hex
                {
                    consume();
                    text += current();

                    while (std::isxdigit(peek(1)))
                    {
                        consume();
                        text += current();
                        if (isDigitSeparator(peek(1)))
                        {
                            consume();
                        }
                    }
                }
                else if (peek(1) == 'b') // binary
                {
                    consume();
                    text += current();

                    while (peek(1) == '0' || peek(1) == '1')
                    {
                        consume();
                        text += current();
                        if (isDigitSeparator(peek(1)))
                        {
                            consume();
                        }
                    }
                }
                else // octal
                {
                    while (peek(1) >= '0' && peek(1) <= '7')
                    {
                        consume();
                        text += current();
                        if (isDigitSeparator(peek(1)))
                        {
                            consume();
                        }
                    }
                }
            }
            else // decimal
            {
                if (isDigitSeparator(peek(1)))
                {
                    consume();
                }

                while (std::isdigit(peek(1)))
                {
                    consume();
                    text += current();
                    if (isDigitSeparator(peek(1)))
                    {
                        consume();
                    }
                }
            }
            return Token(std::move(text), TokenType::IntegerLiteral, start, mSourceLocation);
        }

        if (std::isspace(current()))
        {
            return std::nullopt;
        }

        switch (current())
        {
            case '-':
                if (peek(1) == '>')
                {
                    consume();
                    return Token("->", TokenType::RightArrow, start, mSourceLocation);
                }
                return Token(std::string(1, current()), TokenType::Error, start, mSourceLocation);

            case '(':
                return Token("(", TokenType::LeftParen, start, mSourceLocation);
            case ')':
                return Token(")", TokenType::RightParen, start, mSourceLocation);

            case '{':
                return Token("{", TokenType::LeftBrace, start, mSourceLocation);
            case '}':
                return Token("}", TokenType::RightBrace, start, mSourceLocation);

            case ';':
                return Token(";", TokenType::Semicolon, start, mSourceLocation);

            default:
                return Token(std::string(1, current()), TokenType::Error, start, mSourceLocation);
        }
    }
}