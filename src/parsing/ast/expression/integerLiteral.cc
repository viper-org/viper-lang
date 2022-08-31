#include <parsing/ast/expression/integerLiteral.hh>

IntegerLiteral::IntegerLiteral(long long value)
    :_value(value)
{
    _nodeType = ASTNodeType::Integer;
    _type = types.at("int32");
}

void IntegerLiteral::Print(std::ostream& stream, int indent) const
{
    stream << std::string(indent, ' ') << "<Integer-Literal>: " <<_value;
}

Codegen::Value* IntegerLiteral::Generate(Codegen::Module&, Codegen::Builder& builder, bool)
{
    return builder.CreateIntLiteral(_value, _type);
}