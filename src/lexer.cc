#include "token.hh"
#include <lexer.hh>
#include <iostream>
#include <unordered_set>

static const std::unordered_set<std::string_view> keywords = {
    "return", "if", "else", "for", "do", "while", "break", "goto", 
    "i8", "i16", "i32", "i64", "bool", "void"
};

std::string_view lexer::text;
unsigned int     lexer::position;
unsigned int     lexer::line;

char lexer::current()
{
    return text[position];
}

char lexer::next()
{
    return text[position++];
}

char lexer::peek(int offset)
{
    return text[position + offset];
}

std::vector<token> lexer::lex(std::string_view file)
{
    line = 0;
    text = file;
    std::vector<token> result;

    while(position != text.length())
    {
        std::optional<token> tok = next_token();
        if(tok.has_value())
            result.push_back(tok.value());
        next();
    }
    result.push_back(token(token_type::eof, "", line));

    return result;
}

std::optional<token> lexer::next_token()
{
    char c = current();
    if(std::isalpha(c))
    {
        std::string value(1, c);
        while(std::isalnum(peek(1)) || peek(1) == '_')
        {
            next();
            value += current();
        }
        if(keywords.find(value) != keywords.end())
            return token(token_type::keyword, value, line);
        return token(token_type::identifier, value, line);
    }
    else if(std::isdigit(c))
    {
        std::string value(1, c);
        while(std::isdigit(peek(1)))
        {
            next();
            value += current();
        }
        return token(token_type::integer, value, line);
    }
    else if(c == '+')
    {
        if(peek(1) == '+')
        {
            next();
            return token(token_type::increment, "++", line);
        }
        else if(peek(1) == '=')
        {
            next();
            return token(token_type::increment_assign, "+=", line);
        }
        return token(token_type::plus, "+", line);
    }
    else if(c == '-')
    {
        if(peek(1) == '>')
        {
            next();
            return token(token_type::right_arrow, "->", line);
        }
        else if(peek(1) == '-')
        {
            next();
            return token(token_type::decrement, "--", line);
        }
        else if(peek(1) == '=')
        {
            next();
            return token(token_type::decrement_assign, "-=", line);
        }
        return token(token_type::minus, "-", line);
    }
    else if(c == '*')
        return token(token_type::star, "*", line);
    else if(c == '/')
    {
        if(peek(1) == '/')
        {
            while(current() != '\n' && current() != '\r')
                next();
            return std::nullopt;
        }
        return token(token_type::slash, "/", line);
    }
    else if(c == '(')
        return token(token_type::lparen, "(", line);
    else if(c == ')')
        return token(token_type::rparen, ")", line);
    else if(c == '{')
        return token(token_type::lbracket, "{", line);
    else if(c == '}')
        return token(token_type::rbracket, "}", line);
    else if(c == '@')
        return token(token_type::asperand, "@", line);
    else if(c == ';')
        return token(token_type::semicolon, ";", line);
    else if(c == ',')
        return token(token_type::comma, ",", line);
    else if(c == ':')
        return token(token_type::colon, ":", line);
    else if(c == '=')
    {
        if(peek(1) == '=')
        {
            next();
            return token(token_type::equal, "==", line);
        }
        return token(token_type::assignment, "=", line);
    }
    else if(c == '<')
    {
        if(peek(1) == '=')
        {
            next();
            return token(token_type::less_equal, "<=", line);
        }
        return token(token_type::less_than, "<", line);
    }
    else if(c == '>')
    {
        if(peek(1) == '=')
        {
            next();
            return token(token_type::greater_equal, ">=", line);
        }
        return token(token_type::greater_than, ">", line);
    }
    else if(c == '!')
    {
        if(peek(1) == '=')
        {
            next();
            return token(token_type::not_equal, "!=", line);
        }
    }
    else if (c == '\'')
    {
        next();
        std::string value = "'";
        value += next();
        return token(token_type::character, value + "'", line);
    }
    else if(c == '"')
    {
        std::string value;
        while(current() != '"')
            value += next();
        return token(token_type::string, value, line);
    }
    else if(c == '\n' || c == '\r')
        line++;
    return std::nullopt;
}