#include <lexing/token.hh>

token::token(tokenType type, const std::string& text,
    const unsigned int start, const unsigned int end,
    const unsigned int line_number, const unsigned int col_number)
    :type(type), text(text), start(start), end(end), line_number(line_number), col_number(col_number)
{
}

std::string token::type_as_string() const
{
    switch(type)
    {
        case tokenType::asperand:
            return "asperand";
        case tokenType::identifier:
            return "identifier";
        case tokenType::left_paren:
            return "left_paren";
        case tokenType::right_paren:
            return "right_paren";
        case tokenType::right_arrow:
            return "right_arrow";
        case tokenType::type:
            return "type";
        case tokenType::left_bracket:
            return "left_bracket";
        case tokenType::right_bracket:
            return "right_bracket";
        case tokenType::keyword_return:
            return "keyword_return";
        case tokenType::integer_literal:
            return "integer_literal";
        case tokenType::semicolon:
            return "semicolon";
    }
    return "";
}

std::ostream& operator<<(std::ostream& stream, token tok)
{
    stream << tok.line_number << ":" << tok.col_number << " - " << tok.type_as_string() << "(" << tok.text << ")";
    return stream;
}