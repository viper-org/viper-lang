#include "lexing/token.hh"
#include <lexing/lexer.hh>
#include <type/types.hh>
#include <diagnostics.hh>
#include <unordered_map>

namespace Lexing
{
    std::unordered_map<std::string_view, TokenType> keywords = {
        { "return", TokenType::Return },
        { "let",    TokenType::Let },
        { "true",   TokenType::True },
        { "false",  TokenType::False },
        { "import", TokenType::Import },
        { "if",     TokenType::If },
        { "else",   TokenType::Else },
        { "while",  TokenType::While },
        { "struct", TokenType::Struct },
        { "class", TokenType::Class },
        { "new", TokenType::New },
        { "as", TokenType::As },
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

            if(auto it = types.find(value); it != types.end())
                return Token(TokenType::Type, value, start, _position + 1, _lineNumber, _colNumber);

            if(auto it = keywords.find(value); it != keywords.end())
                return Token(keywords.find(value)->second, value, start, _position + 1, _lineNumber, _colNumber);
            
            return Token(TokenType::Identifier, value, start, _position + 1, _lineNumber, _colNumber);
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

            return Token(TokenType::Integer, value, start, _position + 1, _lineNumber, _colNumber);
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
                return Token(TokenType::LeftParen, "(", _position, _position + 1, _lineNumber, _colNumber);
            case ')':
                return Token(TokenType::RightParen, ")", _position, _position + 1, _lineNumber, _colNumber);

            case '{':
                return Token(TokenType::LeftBracket, "{", _position, _position + 1, _lineNumber, _colNumber);
            case '}':
                return Token(TokenType::RightBracket, "}", _position, _position + 1, _lineNumber, _colNumber);

            case '[':
                return Token(TokenType::LeftSquareBracket, "[", _position, _position + 1, _lineNumber, _colNumber);
            case ']':
                return Token(TokenType::RightSquareBracket, "]", _position, _position + 1, _lineNumber, _colNumber);

            case '<':
                return Token(TokenType::LeftAngleBracket, "<", _position, _position + 1, _lineNumber, _colNumber);
            case '>':
                return Token(TokenType::RightAngleBracket, ">", _position, _position + 1, _lineNumber, _colNumber);


            case '+':
            {
                if(Peek(1) == '=')
                {
                    Consume();
                    return Token(TokenType::PlusEquals, "+=", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Plus, "+", _position, _position + 1, _lineNumber, _colNumber);
            }
            case '-':
            {
                if(Peek(1) == '=')
                {
                    Consume();
                    return Token(TokenType::MinusEquals, "-=", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Minus, "-", _position, _position + 1, _lineNumber, _colNumber);
            }
            case '*':
            {
                if(Peek(1) == '=')
                {
                    Consume();
                    return Token(TokenType::StarEquals, "*=", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Star, "*", _position, _position + 1, _lineNumber, _colNumber);
            }
            case '/':
            {
                if(Peek(1) == '/')
                {
                    while(Current() != '\n')
                        Consume();
                    return std::nullopt;
                }
                else if(Peek(1) == '*')
                {
                    _position += 2;
                    while(Current() != '*' && Peek(1) != '/')
                        Consume();
                    Consume();
                    return std::nullopt;
                }
                else if(Peek(1) == '=')
                {
                    Consume();
                    return Token(TokenType::SlashEquals, "/=", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Slash, "/", _position, _position + 1, _lineNumber, _colNumber);
            }

            case '|':
            {
                if(Peek(1) == '|')
                {
                    Consume();
                    return Token(TokenType::DoublePipe, "||", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Pipe, "|", _position, _position + 1, _lineNumber, _colNumber);
            }
            case '&':
            {
                if(Peek(1) == '&')
                {
                    Consume();
                    return Token(TokenType::DoubleAmpersand, "&&", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Ampersand, "&", _position, _position + 1, _lineNumber, _colNumber);
            }

            case '=':
            {
                if(Peek(1) == '=')
                {
                    Consume();
                    return Token(TokenType::DoubleEquals, "==", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Equals, "=", _position, _position + 1, _lineNumber, _colNumber);
            }

            case '!':
            {
                if(Peek(1) == '=')
                {
                    Consume();
                    return Token(TokenType::BangEquals, "!=", _position - 1, _position + 1, _lineNumber, _colNumber);
                }
                return Token(TokenType::Bang, "!", _position, _position + 1, _lineNumber, _colNumber);
            }


            case ';':
                return Token(TokenType::Semicolon, ";", _position, _position + 1, _lineNumber, _colNumber);
            case ',':
                return Token(TokenType::Comma, ",", _position, _position + 1, _lineNumber, _colNumber);
            case '.':
                return Token(TokenType::Dot, ".", _position, _position + 1, _lineNumber, _colNumber);


            case '#':
                return Token(TokenType::Hash, "#", _position, _position + 1, _lineNumber, _colNumber);
            case '@':
                return Token(TokenType::Asperand, "@", _position, _position + 1, _lineNumber, _colNumber);

            
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