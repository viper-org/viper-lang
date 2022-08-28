#include <parsing/ast/statement/variableDeclaration.hh>
#include <globals.hh>

VariableDeclaration::VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> value)
    :_name(name), _value(std::move(value))
{
    _nodeType = ASTNodeType::VariableDeclaration;
}

void VariableDeclaration::Print(std::ostream& stream, int indent) const
{
    stream << std::string(indent, ' ') << "<Variable-Declaration>:\n";
    stream << std::string(indent, ' ') << "Name: " << _name;
    if(_value)
    {
        stream << "\n" << std::string(indent, ' ') << "Value:\n";
        _value->Print(stream, indent + 2);
    }
}

Codegen::Value* VariableDeclaration::Generate(Codegen::Module& module, Codegen::Builder& builder)
{
    Codegen::AllocaInst* alloca = builder.CreateAlloca();
    namedValues[_name] = alloca;

    if(_value)
    {
        Codegen::Value* valueCodegen = _value->Generate(module, builder);
        return builder.CreateStore(valueCodegen, alloca);
    }

    return alloca;
}