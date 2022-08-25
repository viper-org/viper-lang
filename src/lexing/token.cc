#include <lexing/token.hh>

Token::Token(TokenType type, const std::string& text,
    const unsigned int start, const unsigned int end,
    const unsigned int lineNumber, const unsigned int colNumber)
    :_type(type), _text(text), _start(start), _end(end), _lineNumber(lineNumber), _colNumber(colNumber)
{
}

std::string Token::TypeAsString() const
{
    switch(_type)
    {
        case TokenType::LeftParen:
            return "LeftParen";
        case TokenType::RightParen:
            return "RightParen";
        case TokenType::LeftBracket:
            return "LeftBracket";
        case TokenType::RightBracket:
            return "RightBracket";
        case TokenType::Integer:
            return "Integer";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Return:
            return "Return";
        case TokenType::Type:
            return "Type";
        case TokenType::Asperand:
            return "Asperand";
        case TokenType::RightArrow:
            return "RightArrow";
        case TokenType::Semicolon:
            return "Semicolon";
    }
    return "";
}

TokenType Token::GetType() const
{
    return _type;
}

std::string Token::GetText() const
{
    return _text;
}


unsigned int Token::GetStart() const
{
    return _start;
}

unsigned int Token::GetEnd() const
{
    return _end;
}

unsigned int Token::GetLine() const
{
    return _lineNumber;
}

unsigned int Token::GetCol() const
{
    return _colNumber;
}

std::ostream& operator<<(std::ostream& stream, Token token)
{
    stream << token.GetLine() << ":" << token.GetCol() << " - " << token.TypeAsString() << "(" << token._text << ")";
    return stream;
}