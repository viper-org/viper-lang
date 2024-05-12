// Copyright 2024 solar-mist

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "type/Type.h"

#include <format>
#include <unordered_map>

namespace lexing
{
    Lexer::Lexer(const std::string& text, diagnostic::Diagnostics& diag)
        : mText(text)
        , mDiag(diag)
        , mPosition(0)
    {
    }

    const std::unordered_map<std::string_view, TokenType> keywords = {
        { "func",    TokenType::FuncKeyword },
        { "return",  TokenType::ReturnKeyword },
        { "let",     TokenType::LetKeyword },
        { "global",  TokenType::GlobalKeyword },
        { "if",      TokenType::IfKeyword },
        { "else",    TokenType::ElseKeyword },
        { "while",   TokenType::WhileKeyword },
        { "for",     TokenType::ForKeyword },
        { "break",  TokenType::BreakKeyword },
        { "true",    TokenType::TrueKeyword },
        { "false",   TokenType::FalseKeyword },
        { "nullptr", TokenType::NullptrKeyword },
        { "struct",  TokenType::StructKeyword },
        { "private", TokenType::PrivateKeyword },
        { "import",  TokenType::ImportKeyword },
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
        mColumn++;
        if (mText[mPosition + 1] == '\n')
        {
            mColumn = 0;
            mLine++;
        }
        return mText[mPosition++];
    }

    char Lexer::peek(int offset)
    {
        return mText[mPosition + offset];
    }

    SourceLocation Lexer::location()
    {
        return {mColumn, mLine, mPosition};
    }

    std::optional<Token> Lexer::nextToken()
    {
        SourceLocation start = location();

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
                return Token(keywords.at(text), start, location());
            }

            if (Type::Exists(text))
            {
                return Token(TokenType::Type, std::move(text), start, location());
            }

            if (text.length() >= 2 && text[0] == '_' && std::isupper(text[1]))
            {
                mDiag.compilerError(start, location(), std::format("Identifier '{}{}{}' contains a reserved sequence '{}{}{}'",
                    fmt::bold, text, fmt::defaults, fmt::bold, text.substr(0,2), fmt::defaults));
            }

            return Token(TokenType::Identifier, std::move(text), start, location());
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
                    }
                }
                else // octal
                {
                    while (peek(1) >= '0' && peek(1) <= '7')
                    {
                        consume();
                        text += current();
                    }
                }
            }
            else // decimal
            {
                while (std::isdigit(peek(1)))
                {
                    consume();
                    text += current();
                }
            }
            return Token(TokenType::IntegerLiteral, std::move(text), start, location());
        }
        
        if (std::isspace(current())) // Newline, tab, space etc
        {
            return std::nullopt;
        }

        switch(current())
        {
            case '(':
                return Token(TokenType::LeftParen, start, location());
            case ')':
                return Token(TokenType::RightParen, start, location());

            case '{':
                return Token(TokenType::LeftBracket, start, location());
            case '}':
                return Token(TokenType::RightBracket, start, location());

            case '[':
                return Token(TokenType::LeftSquareBracket, start, location());
            case ']':
                return Token(TokenType::RightSquareBracket, start, location());

            case ';':
                return Token(TokenType::Semicolon, start, location());
            case ':':
                return Token(TokenType::Colon, start, location());
            case ',':
                return Token(TokenType::Comma, start, location());
            case '.':
                return Token(TokenType::Dot, start, location());

            case '@':
                return Token(TokenType::Asperand, start, location());

            case '=':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::DoubleEquals, start, location());
                }
                return Token(TokenType::Equals, start, location());
            
            case '+':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::PlusEquals, start, location());
                }
                return Token(TokenType::Plus, start, location());
            case '-':
                if (peek(1) == '>')
                {
                    consume();
                    return Token(TokenType::RightArrow, start, location());
                }
                else if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::MinusEquals, start, location());
                }
                return Token(TokenType::Minus, start, location());

            case '!':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::BangEquals, start, location());
                }
                break;

            case '<':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::LessEqual, start, location());
                }
                return Token(TokenType::LessThan, start, location());
            case '>':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::GreaterEqual, start, location());
                }
                return Token(TokenType::GreaterThan, start, location());

            case '|':
                return Token(TokenType::Pipe, start, location());
            case '&':
                return Token(TokenType::Ampersand, start, location());
            case '^':
                return Token(TokenType::Caret, start, location());
            case '~':
                return Token(TokenType::Tilde, start, location());
            case '*':
                return Token(TokenType::Star, start, location());

            case '"':
            {
                consume();
                std::string value;
                while(current() != '"')
                {
                    switch(current())
                    {
                        case '\\':
                        {
                            consume();
                            switch(current())
                            {
                                case 'n':
                                    value += '\n';
                                    break;
                                case '\'':
                                    value += '\'';
                                    break;
                                case '\"':
                                    value += '\"';
                                    break;
                                case '\\':
                                    value += '\\';
                                    break;
                                case '0':
                                    value += '\0';
                                    break;
                                default:
                                {
                                    mDiag.compilerError(start, location(), std::format("Unknown escape sequence '{}\\{}{}' in string",
                                        fmt::bold, current(), fmt::defaults));
                                }
                            }
                            break;
                        }
                        default:
                            value += current();
                    }
                    consume();
                }
                return Token(TokenType::StringLiteral, value, start, location());
            }
        }

        return Token(TokenType::Error, std::string(1, current()), start, location()); // Unknown character
    }
}