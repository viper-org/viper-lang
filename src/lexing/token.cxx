#include <lexing/token.hxx>
#include <ostream>
#include <string>

namespace Viper
{
    namespace Lexing
    {
        Token::Token(TokenType type,
        const unsigned int start, const unsigned int end,
        const unsigned int lineNumber, const unsigned int colNumber)
            :_type(type), _start(start), _end(end), _lineNumber(lineNumber), _colNumber(colNumber)
        {
        }

        TokenType Token::getType() const
        {
            return _type;
        }

        unsigned int Token::getStart() const
        {
            return _start;
        }

        unsigned int Token::getEnd() const
        {
            return _end;
        }
        
        unsigned int Token::getLineNumber() const
        {
            return _lineNumber;
        }

        unsigned int Token::getColNumber() const
        {
            return _colNumber;
        }

        std::string Token::typeAsString() const
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
                case TokenType::Plus:
                    return "Plus";
                case TokenType::Minus:
                    return "Minus";
                case TokenType::Star:
                    return "Star";
                case TokenType::Slash:
                    return "Slash";
                case TokenType::Integer:
                    return "Integer";
                case TokenType::Asperand:
                    return "Asperand";
                case TokenType::RightArrow:
                    return "RightArrow";
                case TokenType::Semicolon:
                    return "Semicolon";
                case TokenType::EndOfFile:
                    return "EndOfFile";
                case TokenType::Identifier:
                    return "Identifier";
                case TokenType::BadToken:
                    return "BadToken";
            }
        }

        std::ostream& operator<<(std::ostream& stream, Token token)
        {
            stream << token._lineNumber << ":" << token._colNumber << " - " << token.typeAsString() << "(" << token.getStart() << ", " << token.getEnd() << ")";
            return stream;
        }
    }
}