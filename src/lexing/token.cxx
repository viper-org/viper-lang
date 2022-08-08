#include <lexing/token.hxx>
#include <ostream>
#include <string>

namespace Sketch
{
    namespace Lexing
    {
        Token::Token(TokenType type, const std::string text, unsigned int lineNumber)
            :_type(type), _text(text), _lineNumber(lineNumber)
        {
        }

        TokenType Token::getType() const
        {
            return _type;
        }

        std::string_view Token::getText() const
        {
            return _text;
        }
        
        unsigned int Token::getLineNumber() const
        {
            return _lineNumber;
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
            }
        }

        std::ostream& operator<<(std::ostream& stream, Token token)
        {
            stream << "Line " << token.getLineNumber() << ": " << token.typeAsString() << "(" << token.getText() << ")";
            return stream;
        }
    }
}