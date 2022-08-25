#ifndef VIPER_TOKEN_HH
#define VIPER_TOKEN_HH
#include <string>
#include <ostream>

enum class TokenType
{
    LeftParen, RightParen,
    LeftBracket, RightBracket,

    Integer,

    Identifier,

    Return,

    Type,

    Asperand, RightArrow,

    Semicolon,
};

class Token
{
public:
    Token(TokenType type, const std::string& text,
    const unsigned int start, const unsigned int end,
    const unsigned int lineNumber, const unsigned int colNumber);

    std::string TypeAsString() const;

    TokenType GetType() const;
    std::string GetText() const;
    
    unsigned int GetStart() const;
    unsigned int GetEnd() const;
    unsigned int GetLine() const;
    unsigned int GetCol() const;

    friend std::ostream& operator<<(std::ostream& stream, Token token);
private:
    TokenType _type;
    std::string _text;

    unsigned int _start;
    unsigned int _end;

    unsigned int _lineNumber;
    unsigned int _colNumber;
};

#endif