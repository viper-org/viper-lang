// Copyright 2024 solar-mist

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "type/Type.h"

#include <iostream>
#include <unordered_map>

namespace lexing
{
    Lexer::Lexer(const std::string& text)
        :mText(text), mPosition(0)
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
        { "true",    TokenType::TrueKeyword },
        { "false",   TokenType::FalseKeyword },
        { "nullptr", TokenType::NullptrKeyword },
        { "struct",  TokenType::StructKeyword },
        { "private", TokenType::PrivateKeyword },
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

            if (text.length() >= 2 && text[0] == '_' && std::isupper(text[1]))
            {
                std::cerr << "Identifiers cannot begin with _ and an uppercase character. (" << text << ")\n";
                std::exit(1);
            }

            return Token(TokenType::Identifier, std::move(text));
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
            return Token {TokenType::IntegerLiteral, std::move(text)};
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

            case '[':
                return Token(TokenType::LeftSquareBracket);
            case ']':
                return Token(TokenType::RightSquareBracket);

            case ';':
                return Token(TokenType::Semicolon);
            case ':':
                return Token(TokenType::Colon);
            case ',':
                return Token(TokenType::Comma);
            case '.':
                return Token(TokenType::Dot);

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
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::PlusEquals);
                }
                return Token(TokenType::Plus);
            case '-':
                if (peek(1) == '>')
                {
                    consume();
                    return Token(TokenType::RightArrow);
                }
                else if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::MinusEquals);
                }
                return Token(TokenType::Minus);

            case '!':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::BangEquals);
                }
                break;

            case '<':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::LessEqual);
                }
                return Token(TokenType::LessThan);
            case '>':
                if (peek(1) == '=')
                {
                    consume();
                    return Token(TokenType::GreaterEqual);
                }
                return Token(TokenType::GreaterThan);

            case '|':
                return Token(TokenType::Pipe);
            case '&':
                return Token(TokenType::Ampersand);
            case '^':
                return Token(TokenType::Caret);
            case '~':
                return Token(TokenType::Tilde);
            case '*':
                return Token(TokenType::Star);

            case '"':
            {
                consume();
                std::string value;
                int start = mPosition;
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
                                    std::cerr << "Unknown escape sequence in string. Stop\n";
                                    std::exit(1);
                                }
                            }
                            break;
                        }
                        default:
                            value += current();
                    }
                    consume();
                }
                return Token(TokenType::StringLiteral, value);
            }
        }

        return Token(TokenType::Error, std::string(1, current())); // Unknown character
    }
}