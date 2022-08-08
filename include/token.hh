#ifndef QRK_TOKEN_HH
#define QRK_TOKEN_HH
#include <ostream>
#include <string>

enum class token_type
{
    lparen, rparen,
    lbracket, rbracket,
    lsqbracket, rsqbracket,

    plus, minus, star, slash,

    equal, not_equal,
    less_than, greater_than,
    less_equal, greater_equal,

    assignment, increment_assign, decrement_assign,

    increment, decrement,

    integer, character, string,

    asperand, ampersand, hash, right_arrow,

    semicolon, comma, colon,

    identifier, keyword,

    eof
};

struct token
{
    token_type type;
    std::string text;
    unsigned int line;

    token(token_type, std::string, unsigned int);

    std::string get_type();

    friend std::ostream& operator<<(std::ostream&, token);
};

#endif