#include "lexing/token.hh"
#include <cctype>
#include <lexing/lexer.hh>
#include <diagnostics.hh>
#include <optional>
#include <unordered_map>

std::unordered_map<std::string_view, tokenType> keywords = {
    { "return", tokenType::keyword_return },
    { "int32", tokenType::type }, // TODO: Add proper type support
};

lexer::lexer(const std::string& text)
    :text(text), position(0), line_number(1), col_number(1), line_begin(&this->text[position])
{
}

std::vector<token> lexer::lex()
{
    std::vector<token> tokens;

    while(position < text.length())
    {
        std::optional<token> tok = next_token();
        if(tok.has_value())
            tokens.push_back(tok.value());
        consume();
    }

    return tokens;
}

char lexer::current() const
{
    return text[position];
}

char lexer::consume()
{
    return text[position++];
}

char lexer::peek(int offset) const
{
    return text[position + offset];
}

std::optional<token> lexer::next_token()
{
    if(std::isalpha(current()) || current() == '_')
    {
        unsigned int start = position;
        std::string value = std::string(1, current());
        
        while(std::isalnum(peek(1)) || peek(1) == '_')
        {
            consume();
            value += current();
        }

        if(auto it = keywords.find(value); it != keywords.end())
            return token(keywords.find(value)->second, value, start, position + 1, line_number, col_number);
        
        return token(tokenType::identifier, value, start, position + 1, line_number, col_number);
    }

    if(isdigit(current()))
    {
        unsigned int start = position;
        std::string value = std::string(1, current());

        while(std::isalnum(peek(1)) || peek(1) == '_')
        {
            consume();
            value += current();
        }

        return token(tokenType::integer_literal, value, start, position + 1, line_number, col_number);
    }

    switch(current())
    {
        case '\n':
        {
            col_number = 0;
            ++line_number;
            line_begin = &text[position + 1];
            return std::nullopt;
        }
        case ' ':
            return std::nullopt;

        case '@':
            return token(tokenType::asperand, "@", position, position + 1, line_number, col_number);
        

        case '(':
            return token(tokenType::left_paren, "(", position, position + 1, line_number, col_number);
        case ')':
            return token(tokenType::right_paren, ")", position, position + 1, line_number, col_number);

        case '{':
            return token(tokenType::left_bracket, "{", position, position + 1, line_number, col_number);
        case '}':
            return token(tokenType::right_bracket, "}", position, position + 1, line_number, col_number);

        case ';':
            return token(tokenType::semicolon, ";", position, position + 1, line_number, col_number);


        case '-':
        {
            if(peek(1) == '>')
            {
                consume();
                return token(tokenType::right_arrow, "->", position - 1, position + 1, line_number, col_number);
            }
        }
        

        default:
            char* line_end;
            int offset = 0;
            while(peek(offset) != '\n')
                offset++;
            line_end = &text[position + offset];
            diagnostics::compiler_error(line_number, col_number, "stray '" + std::string(1, current()) + "' found in program", &text[position], &text[position + 1], line_begin, line_end);
    }
}