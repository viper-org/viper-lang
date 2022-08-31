#include <parsing/ast/statement/variableDeclaration.hh>
#include <globals.hh>

VariableDeclaration::VariableDeclaration(Type* type, const std::string& name, std::unique_ptr<ASTNode> value)
    :_type(type), _name(name), _value(std::move(value))
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

Codegen::Value* VariableDeclaration::Generate(Codegen::Module& module, Codegen::Builder& builder, bool isStatement)
{
    Codegen::AllocaInst* alloca = builder.CreateAlloca(_type);
    namedValues[_name] = alloca;

    if(_value)
    {
        Codegen::Value* valueCodegen = _value->Generate(module, builder);
        valueCodegen->SetType(_type);
        return builder.CreateStore(valueCodegen, alloca, isStatement);
    }

    return alloca;
}