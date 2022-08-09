#include "lexing/token.hxx"
#include <iostream>
#include <lexing/lexer.hxx>
#include <diagnostics.hxx>
#include <optional>

namespace Quark
{
    namespace Lexing
    {
        Lexer::Lexer(std::string text)
            :_text(text),  _position(0), _lineNumber(1), _colNumber(1), _lineBegin(&_text[_position])
        {
        }

        std::vector<Token> Lexer::Lex()
        {
            std::vector<Token> tokens;

            while(_position != _text.length())
            {
                std::optional<Token> token = NextToken();
                if(token.has_value())
                    tokens.push_back(token.value());
                Consume();
            }
            tokens.push_back(Token(TokenType::EndOfFile, "", _lineNumber, _colNumber));

            return tokens;
        }

        char Lexer::Current() const
        {
            return _text[_position];
        }

        char Lexer::Consume()
        {
            _colNumber++;
            return _text[_position++];
        }

        char Lexer::Peek(const int offset) const
        {
            return _text[_position + offset];
        }

        std::optional<Token> Lexer::NextToken()
        {
            char current = Current();

            if(std::isalpha(current))
            {
                std::string value(1, current);
                while(std::isalnum(Peek(1)) || Peek(1) == '_')
                {
                    Consume();
                    value += Current();
                }
                return Token(TokenType::Identifier, value, _lineNumber, _colNumber);
            }
            else if(std::isdigit(current))
            {
                std::string value(1, current);
                while(std::isdigit(Peek(1)))
                {
                    Consume();
                    value += Current();
                }
                return Token(TokenType::Integer, value, _lineNumber, _colNumber);
            }
            switch(current)
            {
                case '(':
                    return Token(TokenType::LeftParen, "(", _lineNumber, _colNumber);
                case ')':
                    return Token(TokenType::RightParen, ")", _lineNumber, _colNumber);
                
                case '{':
                    return Token(TokenType::LeftBracket, "{", _lineNumber, _colNumber);
                case '}':
                    return Token(TokenType::RightBracket, "}", _lineNumber, _colNumber);

                case '+':
                    return Token(TokenType::Plus, "+", _lineNumber, _colNumber);
                case '-':
                {
                    if(Peek(1) == '>')
                    {
                        Consume();
                        return Token(TokenType::RightArrow, "->", _lineNumber, _colNumber);
                    }
                    return Token(TokenType::Minus, "-", _lineNumber, _colNumber);
                }
                case '*':
                    return Token(TokenType::Star, "*", _lineNumber, _colNumber);
                case '/':
                    return Token(TokenType::Slash, "/", _lineNumber, _colNumber);
                
                case '@':
                    return Token(TokenType::Asperand, "@", _lineNumber, _colNumber);

                case ';':
                    return Token(TokenType::Semicolon, ";", _lineNumber, _colNumber);

                case '\n':
                    _colNumber = 0;
                    _lineNumber++;
                    _lineBegin = &_text[_position + 1];
                    return std::nullopt;

                case ' ':
                    return std::nullopt;

                default:
                    char* lineEnd;
                    int offset = 0;
                    while(Peek(offset) != '\n')
                        offset++;
                    lineEnd = &_text[_position + offset];
                    Diagnostics::CompilerError(_lineNumber, _colNumber, "stray '" + std::string(1, current) + "' found in program", &_text[_position], &_text[_position + 1], _lineBegin, lineEnd);
            }
        }
    }
}