#include <parsing/ast/statement/return.hh>

namespace Parsing
{
    ReturnStatement::ReturnStatement(std::unique_ptr<ASTNode> value, std::shared_ptr<Type> returnType)
        :ASTNode(ASTNodeType::ReturnStatement), _value(std::move(value)), _returnType(returnType)
    {
    }

    void ReturnStatement::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Return-Statement>";
        if(_value)
        {
            stream << ":\n" << std::string(indent, ' ') << "Value:\n";
            _value->Print(stream, indent + 2);
        }
    }

    std::shared_ptr<Type> ReturnStatement::GetReturnType() const
    {
        return _returnType;
    }

    SSA::Value* ReturnStatement::Emit(SSA::Builder& builder, bool)
    {
        SSA::Value* value = nullptr;
        if(_value)
            value = _value->Emit(builder);

        return builder.CreateRet(value);
    }
}