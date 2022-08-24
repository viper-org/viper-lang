#ifndef VIPER_TOKEN_HH
#define VIPER_TOKEN_HH
#include <string>
#include <ostream>

enum class tokenType
{
    asperand,

    identifier,

    left_paren,
    right_paren,

    right_arrow,

    type,

    left_bracket,
    right_bracket,

    keyword_return,

    integer_literal,

    semicolon,
};

class token
{
public:
    token(tokenType type, const std::string& text,
    const unsigned int start, const unsigned int end,
    const unsigned int line_number, const unsigned int col_number);

    std::string type_as_string() const;

    friend std::ostream& operator<<(std::ostream& stream, token tok);

    tokenType type;
    std::string text;

    unsigned int start;
    unsigned int end;

    unsigned int line_number;
    unsigned int col_number;
};

#endif