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
            case Lexing::TokenType::LeftSquareBracket:
                return "LeftSquareBracket";
            case Lexing::TokenType::RightSquareBracket:
                return "RightSquareBracket";
            case Lexing::TokenType::Plus:
                return "Plus";
            case Lexing::TokenType::Minus:
                return "Minus";
            case Lexing::TokenType::Star:
                return "Star";
            case Lexing::TokenType::Slash:
                return "Slash";
            case Lexing::TokenType::Integer:
                return "Integer";
            case Lexing::TokenType::Identifier:
                return "Identifier";
            case Lexing::TokenType::Return:
                return "Return";
            case Lexing::TokenType::Let:
                return "Let";
            case Lexing::TokenType::Type:
                return "Type";
            case Lexing::TokenType::Semicolon:
                return "Semicolon";
            case Lexing::TokenType::Equals:
                return "Equals";
            case Lexing::TokenType::Comma:
                return "Comma";
            case TokenType::DoubleEquals:
                return "DoubleEquals";
            case TokenType::Hash:
                return "Hash";
            case TokenType::LeftAngleBracket:
                return "LeftAngleBracket";
            case TokenType::RightAngleBracket:
                return "RightAngleBracket";
            case TokenType::BangEquals:
                return "BangEquals";
            case TokenType::String:
                return "String";
            case TokenType::PlusEquals:
                return "PlusEquals";
            case TokenType::MinusEquals:
                return "MinusEquals";
            case TokenType::StarEquals:
                return "StarEquals";
            case TokenType::SlashEquals:
                return "SlashEquals";
            case TokenType::True:
                return "True";
            case TokenType::False:
                return "False";
            case Lexing::TokenType::Import:
                return "Import";
        }
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