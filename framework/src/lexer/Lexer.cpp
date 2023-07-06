#include <lexer/Lexer.h>
#include <lexer/Token.h>

namespace Lexing
{
    Lexer::Lexer(const std::string& text)
        :mText(text), mPosition(0)
    {
    }

    std::vector<Token> Lexer::lex()
    {
        std::vector<Token> tokens;

        while (mPosition < mText.length())
        {
            std::optional<Token> token = nextToken();
            if(token.has_value())
                tokens.push_back(*token);
            consume();
        }

        return tokens;
    }

    char Lexer::current()
    {
        return mText[mPosition];
    }

    char Lexer::consume()
    {
        return mText[mPosition++];
    }

    char Lexer::peek(int offset)
    {
        return mText[mPosition + offset];
    }

    std::optional<Token> Lexer::nextToken()
    {
        if (std::isalpha(current()) || current() == '_') // Identifier
        {
            std::string text = std::string(1, current());

            while (std::isalnum(peek(1)) || peek(1) == '_')
            {
                consume();
                text += current();
            }

            return Token(TokenType::Identifier, std::move(text));
        }
        
        if (std::isspace(current())) // Newline, tab, space etc
        {
            consume();
            return std::nullopt;
        }

        switch(current())
        {
            case '(':
                return Token(TokenType::LeftParen);
            case ')':
                return Token(TokenType::RightParen);

            case '{':
                return Token(TokenType::LeftBracket);
            case '}':
                return Token(TokenType::RightBracket);
        }

        return Token(TokenType::Error); // Unknown character
    }
}