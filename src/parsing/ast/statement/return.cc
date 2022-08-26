#include <parsing/ast/statement/return.hh>

ReturnStatement::ReturnStatement(std::unique_ptr<ASTNode> value)
    :_value(std::move(value))
{
}

void ReturnStatement::Print(std::ostream& stream, int indent) const
{
    stream << std::string(indent, ' ') << "<Return-Statement>";
    if(_value)
    {
        stream << ":\n" <<std::string(indent, ' ') << "Value:\n";
        _value->Print(stream, indent + 2);
    }
}

Codegen::Value* ReturnStatement::Generate(Codegen::Module& module, Codegen::Builder& builder)
{
    if(_value)
        return builder.CreateRet(_value->Generate(module, builder));

    return builder.CreateRet(nullptr);
}