#include <lexing/lexer.hh>
#include <diagnostics.hh>
#include <optional>
#include <unordered_map>

namespace Lexing
{
    std::unordered_map<std::string_view, TokenType> keywords = {
        { "return", TokenType::Return },
        { "extern", TokenType::Extern },
        { "if",     TokenType::If },
        { "else",   TokenType::Else },
        { "while", TokenType::While },
        { "decay", TokenType::Decay },
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
            std::optional<Token> tok = NextToken();
            if(tok.has_value())
                tokens.push_back(tok.value());
            Consume();
        }

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

    char Lexer::Peek(int offset) const
    {
        return _text[_position + offset];
    }

    std::optional<Token> Lexer::NextToken()
    {
        if(std::isalpha(Current()) || Current() == '_')
        {
            unsigned int start = _position;
            std::string value = std::string(1, Current());
            
            while(std::isalnum(Peek(1)) || Peek(1) == '_')
            {
                Consume();
                value += Current();
            }

            //if(auto it = types.find(value); it != types.end())
            //    return Token(TokenType::Type, value, start, _position + 1, _lineNumber, _colNumber);

            if(auto it = keywords.find(value); it != keywords.end())
                return Token(keywords.find(value)->second, value, start, _position + 1, _lineNumber, _colNumber);
            
            return Token(Lexing::TokenType::Identifier, value, start, _position + 1, _lineNumber, _colNumber);
        }

        if(isdigit(Current()))
        {
            unsigned int start = _position;
            std::string value = std::string(1, Current());

            while(std::isalnum(Peek(1)) || Peek(1) == '_')
            {
                Consume();
                value += Current();
            }

            return Token(Lexing::TokenType::Integer, value, start, _position + 1, _lineNumber, _colNumber);
        }

        switch(Current())
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

            case '[':
                return Token(Lexing::TokenType::LeftSquareBracket, "[", _position, _position + 1, _lineNumber, _colNumber);
            case ']':
                return Token(Lexing::TokenType::RightSquareBracket, "]", _position, _position + 1, _lineNumber, _colNumber);

            case '<':
                return Token(Lexing::TokenType::LeftAngleBracket, "<", _position, _position + 1, _lineNumber, _colNumber);
            case '>':
                return Token(Lexing::TokenType::RightAngleBracket, ">", _position, _position + 1, _lineNumber, _colNumber);


            case '+':
                return Token(Lexing::TokenType::Plus, "+", _position, _position + 1, _lineNumber, _colNumber);
            case '-':
            {
                if(Peek(1) == '>')
                {
                    Consume();
                    return Token(Lexing::TokenType::RightArrow, "->", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(Lexing::TokenType::Minus, "-", _position, _position + 1, _lineNumber, _colNumber);
            }
            case '*':
                return Token(Lexing::TokenType::Star, "*", _position, _position + 1, _lineNumber, _colNumber);
            case '/':
                return Token(Lexing::TokenType::Slash, "/", _position, _position + 1, _lineNumber, _colNumber);

            case '=':
            {
                if(Peek(1) == '=')
                {
                    Consume();
                    return Token(Lexing::TokenType::DoubleEquals, "==", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(Lexing::TokenType::Equals, "=", _position, _position + 1, _lineNumber, _colNumber);
            }


            case ';':
                return Token(Lexing::TokenType::Semicolon, ";", _position, _position + 1, _lineNumber, _colNumber);
            case ',':
                return Token(Lexing::TokenType::Comma, ",", _position, _position + 1, _lineNumber, _colNumber);


            case '@':
                return Token(Lexing::TokenType::Asperand, "@", _position, _position + 1, _lineNumber, _colNumber);
            case '#':
                return Token(Lexing::TokenType::Hash, "#", _position, _position + 1, _lineNumber, _colNumber);

            case '.':
                return Token(Lexing::TokenType::Dot, ".", _position, _position + 1, _lineNumber, _colNumber);

            
            case '"':
            {
                Consume();
                std::string value;
                int start = _position;
                while(Current() != '"')
                {
                    switch(Current())
                {
                    case '\\':
                    {
                        Consume();
                        switch(Current())
                        {
                            case 'n':
                                value += '\n';
                                break;
                            case '\'':
                                value += '\'';
                                break;
                            case '\\':
                                value += '\\';
                                break;
                            case '0':
                                value += '\0';
                                break;
                            default:
                            {
                                char* line_end;
                                int offset = 0;
                                while(Peek(offset) != '\n')
                                    offset++;
                                line_end = &_text[_position + offset];
                                Diagnostics::CompilerError(_lineNumber, _colNumber, "unknown escape sequence: '\\" + std::string(1, Current()) + "'", &_text[_position], &_text[_position + 1], _lineBegin, line_end);
                            }
                        }
                        break;
                    }
                    default:
                        value += Current();
                }
                    Consume();
                }
                return Token(TokenType::String, value, start, _position, _lineNumber, _colNumber);
            }

            case '\'':
            {
                char ch;
                Consume();
                switch(Current())
                {
                    case '\\':
                    {
                        Consume();
                        switch(Current())
                        {
                            case 'n':
                                ch = '\n';
                                break;
                            case '\'':
                                ch = '\'';
                                break;
                            case '\\':
                                ch = '\\';
                                break;
                            case '0':
                                ch = '\0';
                                break;
                            default:
                            {
                                char* line_end;
                                int offset = 0;
                                while(Peek(offset) != '\n')
                                    offset++;
                                line_end = &_text[_position + offset];
                                Diagnostics::CompilerError(_lineNumber, _colNumber, "unknown escape sequence: '\\" + std::string(1, Current()) + "'", &_text[_position], &_text[_position + 1], _lineBegin, line_end);
                            }
                        }
                        break;
                    }
                    default:
                        ch = Current();
                }
                Consume();
                if(Current() != '\'')
                {
                    char* line_end;
                    int offset = 0;
                    while(Peek(offset) != '\n')
                        offset++;
                    line_end = &_text[_position + offset];
                    Diagnostics::CompilerError(_lineNumber, _colNumber, "Missing terminating ' character", &_text[_position], &_text[_position + 1], _lineBegin, line_end);
                }
                return Token(TokenType::Integer, std::to_string((int)ch), _position, _position + 1, _lineNumber, _colNumber);
            }

            default:
                char* line_end;
                int offset = 0;
                while(Peek(offset) != '\n')
                    offset++;
                line_end = &_text[_position + offset];
                Diagnostics::CompilerError(_lineNumber, _colNumber, "stray '" + std::string(1, Current()) + "' found in program", &_text[_position], &_text[_position + 1], _lineBegin, line_end);
        }
    }
}