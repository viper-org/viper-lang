#include <parsing/ast/expression/integerLiteral.hh>

namespace Parsing
{
    IntegerLiteral::IntegerLiteral(long long value)
        :ASTNode(ASTNodeType::Integer), _value(value)
    {
        _type = types.at("int64");
    }

    void IntegerLiteral::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Integer-Literal>: " <<_value;
    }

    long long IntegerLiteral::GetValue() const
    {
        return _value;
    }

    SSA::Value* IntegerLiteral::Emit(SSA::Builder& builder, bool)
    {
        return builder.CreateConstantInt(_value);
    }
}