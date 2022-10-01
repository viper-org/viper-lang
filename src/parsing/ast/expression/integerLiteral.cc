#include <parsing/ast/expression/integerLiteral.hh>

namespace Parsing
{
    IntegerLiteral::IntegerLiteral(long long value)
        :_value(value)
    {
        _nodeType = ASTNodeType::Integer;
    }

    void IntegerLiteral::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Integer-Literal>: " <<_value;
    }

    long long IntegerLiteral::GetValue() const
    {
        return _value;
    }
}