#include <lexing/lexer.hxx>
#include <diagnostics.hxx>
#include <optional>

namespace Quark
{
    namespace Lexing
    {
        Lexer::Lexer(std::string text, std::string_view fileName)
            :_text(text), _fileName(fileName), _position(0), _lineNumber(1), _colNumber(1)
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
            tokens.push_back(Token(TokenType::EndOfFile, "", _lineNumber));

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
                return Token(TokenType::Identifier, value, _lineNumber);
            }
            else if(std::isdigit(current))
            {
                std::string value(1, current);
                while(std::isdigit(Peek(1)))
                {
                    Consume();
                    value += Current();
                }
                return Token(TokenType::Integer, value, _lineNumber);
            }
            switch(current)
            {
                case '(':
                    return Token(TokenType::LeftParen, "(", _lineNumber);
                case ')':
                    return Token(TokenType::RightParen, ")", _lineNumber);
                
                case '{':
                    return Token(TokenType::LeftBracket, "{", _lineNumber);
                case '}':
                    return Token(TokenType::RightBracket, "}", _lineNumber);

                case '+':
                    return Token(TokenType::Plus, "+", _lineNumber);
                case '-':
                {
                    if(Peek(1) == '>')
                    {
                        Consume();
                        return Token(TokenType::RightArrow, "->", _lineNumber);
                    }
                    return Token(TokenType::Minus, "-", _lineNumber);
                }
                case '*':
                    return Token(TokenType::Star, "*", _lineNumber);
                case '/':
                    return Token(TokenType::Slash, "/", _lineNumber);
                
                case '@':
                    return Token(TokenType::Asperand, "@", _lineNumber);

                case ';':
                    return Token(TokenType::Semicolon, ";", _lineNumber);

                case '\n':
                    _colNumber = 0;
                    _lineNumber++;
                    return std::nullopt;

                case ' ':
                    return std::nullopt;

                default:
                    Diagnostics::CompilerError(_fileName, _lineNumber, _colNumber, std::string("Unexpected character: ") + current, &_text[_position], &_text[_position + 1]);
            }
        }
    }
}