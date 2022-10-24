#include <parsing/ast/expression/string.hh>

namespace Parsing
{
    StringLiteral::StringLiteral(const std::string& value)
        :ASTNode(ASTNodeType::String), _value(value)
    {
        _type = std::make_shared<PointerType>(types.at("int8"));
    }

    void StringLiteral::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<String-Literal>: " << _value;
    }

    std::string StringLiteral::GetValue() const
    {
        return _value;
    }

    SSA::Value* StringLiteral::Emit(SSA::Builder& builder, Environment*, bool)
    {
        return SSA::GlobalString::Create(builder.GetModule(), _value);
    }
}