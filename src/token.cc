#include <token.hh>

token::token(token_type type, std::string text, unsigned int line)
    :type(type), text(text), line(line)
{
}

std::string token::get_type()
{
    switch(type)
    {
        case token_type::lparen:
            return "LPAREN";
        case token_type::rparen:
            return "RPAREN";
        case token_type::lbracket:
            return "LBRACKET";
        case token_type::rbracket:
            return "RBRACKET";
        case token_type::plus:
            return "PLUS";
        case token_type::minus:
            return "MINUS";
        case token_type::star:
            return "STAR";
        case token_type::slash:
            return "SLASH";
        case token_type::integer:
            return "INTEGER";
        case token_type::character:
            return "CHARACTER";
        case token_type::asperand:
            return "ASPERAND";
        case token_type::right_arrow:
            return "RIGHT_ARROW";
        case token_type::semicolon:
            return "SEMICOLON";
        case token_type::comma:
            return "COMMA";
        case token_type::colon:
            return "COLON";
        case token_type::identifier:
            return "IDENTIFIER";
        case token_type::keyword:
            return "KEYWORD";
        case token_type::eof:
            return "EOF";
        case token_type::equal:
            return "EQUAL";
        case token_type::not_equal:
            return "NOT_EQUAL";
        case token_type::less_than:
            return "LESS_THAN";
        case token_type::greater_than:
            return "GREATER_THAN";
        case token_type::less_equal:
            return "LESS_EQUAL";
        case token_type::greater_equal:
            return "GREATER_EQUAL";
        case token_type::assignment:
            return "ASSIGNMENT";
        case token_type::string:
            return "STRING";
        case token_type::increment_assign:
            return "INCREMENT_ASSIGN";
        case token_type::decrement_assign:
            return "DECREMENT_ASSIGN";
        case token_type::increment:
            return "INCREMENT";
        case token_type::decrement:
            return "DECREMENT";
        }
    return "";
}

std::ostream& operator<<(std::ostream& stream, token tok)
{
    stream << "Line " << tok.line << ": " << tok.get_type() << "(" << tok.text << ")";
    return stream;
}