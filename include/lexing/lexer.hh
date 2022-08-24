#ifndef VIPER_LEXER_HH
#define VIPER_LEXER_HH
#include <lexing/token.hh>
#include <optional>
#include <string>
#include <vector>

class lexer
{
public:
    lexer(const std::string& text);

    std::vector<token> lex();
private:
    std::string text;
    unsigned int position;
    unsigned int line_number;
    unsigned int col_number;
    char* line_begin;

    char current() const;
    char consume();
    char peek(const int offset) const;

    std::optional<token> next_token();
};

#endif