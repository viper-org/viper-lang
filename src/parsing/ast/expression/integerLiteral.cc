#include <parsing/ast/expression/integerLiteral.hh>

IntegerLiteral::IntegerLiteral(long long value)
    :_value(value)
{
}

void IntegerLiteral::Print(std::ostream& stream, int indent) const
{
    stream << std::string(indent, ' ') << "<Integer-Literal>: " <<_value;
}