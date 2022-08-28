#include <lexing/lexer.hh>
#include <diagnostics.hh>
#include <optional>
#include <unordered_map>

namespace Lexing
{
    std::unordered_map<std::string_view, TokenType> keywords = {
        { "return", TokenType::Return },
        { "extern", TokenType::Extern },
        { "int64", TokenType::Type }, // TODO: Add proper type support
    };

    Lexer::Lexer(const std::string& text)
        :_text(text), _position(0), _lineNumber(1), _colNumber(1), _lineBegin(&this->_text[_position])
    {
    }

    std::vector<Token> Lexer::Lex()
    {
        std::vector<Token> tokens;

        while(_position < _text.length())
        {
            std::optional<Token> tok = next_token();
            if(tok.has_value())
                tokens.push_back(tok.value());
            consume();
        }

        return tokens;
    }

    char Lexer::current() const
    {
        return _text[_position];
    }

    char Lexer::consume()
    {
        _colNumber++;
        return _text[_position++];
    }

    char Lexer::peek(int offset) const
    {
        return _text[_position + offset];
    }

    std::optional<Token> Lexer::next_token()
    {
        if(std::isalpha(current()) || current() == '_')
        {
            unsigned int start = _position;
            std::string value = std::string(1, current());
            
            while(std::isalnum(peek(1)) || peek(1) == '_')
            {
                consume();
                value += current();
            }

            if(auto it = keywords.find(value); it != keywords.end())
                return Token(keywords.find(value)->second, value, start, _position + 1, _lineNumber, _colNumber);
            
            return Token(Lexing::TokenType::Identifier, value, start, _position + 1, _lineNumber, _colNumber);
        }

        if(isdigit(current()))
        {
            unsigned int start = _position;
            std::string value = std::string(1, current());

            while(std::isalnum(peek(1)) || peek(1) == '_')
            {
                consume();
                value += current();
            }

            return Token(Lexing::TokenType::Integer, value, start, _position + 1, _lineNumber, _colNumber);
        }

        switch(current())
        {
            case '\n':
            {
                _colNumber = 0;
                ++_lineNumber;
                _lineBegin = &_text[_position + 1];
                return std::nullopt;
            }
            case ' ':
                return std::nullopt;
            

            case '(':
                return Token(Lexing::TokenType::LeftParen, "(", _position, _position + 1, _lineNumber, _colNumber);
            case ')':
                return Token(Lexing::TokenType::RightParen, ")", _position, _position + 1, _lineNumber, _colNumber);

            case '{':
                return Token(Lexing::TokenType::LeftBracket, "{", _position, _position + 1, _lineNumber, _colNumber);
            case '}':
                return Token(Lexing::TokenType::RightBracket, "}", _position, _position + 1, _lineNumber, _colNumber);

            case '+':
                return Token(Lexing::TokenType::Plus, "+", _position, _position + 1, _lineNumber, _colNumber);
            case '-':
            {
                if(peek(1) == '>')
                {
                    consume();
                    return Token(Lexing::TokenType::RightArrow, "->", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(Lexing::TokenType::Minus, "-", _position, _position + 1, _lineNumber, _colNumber);
            }
            case '*':
                return Token(Lexing::TokenType::Star, "*", _position, _position + 1, _lineNumber, _colNumber);
            case '/':
                return Token(Lexing::TokenType::Slash, "/", _position, _position + 1, _lineNumber, _colNumber);

            case '=':
                return Token(Lexing::TokenType::Equals, "=", _position, _position + 1, _lineNumber, _colNumber);


            case ';':
                return Token(Lexing::TokenType::Semicolon, ";", _position, _position + 1, _lineNumber, _colNumber);
            case ',':
                return Token(Lexing::TokenType::Comma, ",", _position, _position + 1, _lineNumber, _colNumber);


            case '@':
                return Token(Lexing::TokenType::Asperand, "@", _position, _position + 1, _lineNumber, _colNumber);
            

            default:
                char* line_end;
                int offset = 0;
                while(peek(offset) != '\n')
                    offset++;
                line_end = &_text[_position + offset];
                Diagnostics::CompilerError(_lineNumber, _colNumber, "stray '" + std::string(1, current()) + "' found in program", &_text[_position], &_text[_position + 1], _lineBegin, line_end);
        }
    }
}