#ifndef QRK_LEXER_HH
#define QRK_LEXER_HH
#include <token.hh>
#include <string>
#include <optional>
#include <vector>

struct lexer
{
    static std::string_view text;
    static unsigned int position;
    static unsigned int line;

    static char current();
    static char next();
    static char peek(int);

    static std::vector<token> lex(std::string_view);
    static std::optional<token> next_token();
};

#endif