#include <lexing/token.hh>

namespace Lexing
{
    Token::Token(Lexing::TokenType type, const std::string& text,
        const unsigned int start, const unsigned int end,
        const unsigned int lineNumber, const unsigned int colNumber)
        :_type(type), _text(text), _start(start), _end(end), _lineNumber(lineNumber), _colNumber(colNumber)
    {
    }

    std::string Token::TypeAsString() const
    {
        switch(_type)
        {
            case Lexing::TokenType::LeftParen:
                return "LeftParen";
            case Lexing::TokenType::RightParen:
                return "RightParen";
            case Lexing::TokenType::LeftBracket:
                return "LeftBracket";
            case Lexing::TokenType::RightBracket:
                return "RightBracket";
            case Lexing::TokenType::Integer:
                return "Integer";
            case Lexing::TokenType::Identifier:
                return "Identifier";
            case Lexing::TokenType::Return:
                return "Return";
            case Lexing::TokenType::Type:
                return "Type";
            case Lexing::TokenType::Asperand:
                return "Asperand";
            case Lexing::TokenType::RightArrow:
                return "RightArrow";
            case Lexing::TokenType::Semicolon:
                return "Semicolon";
        }
        return "";
    }

    Lexing::TokenType Token::GetType() const
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
}