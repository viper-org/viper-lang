#include <parsing/ast/expression/integerLiteral.hh>

IntegerLiteral::IntegerLiteral(long long value)
    :_value(value)
{
    _nodeType = ASTNodeType::Integer;
}

void IntegerLiteral::Print(std::ostream& stream, int indent) const
{
    stream << std::string(indent, ' ') << "<Integer-Literal>: " <<_value;
}

Codegen::Value* IntegerLiteral::Generate(Codegen::Module&, Codegen::Builder& builder)
{
    return builder.CreateIntLiteral(_value);
}