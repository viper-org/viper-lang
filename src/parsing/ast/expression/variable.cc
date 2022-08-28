#include <parsing/ast/expression/variable.hh>
#include <globals.hh>

Variable::Variable(const std::string& name)
    :_name(name)
{
    _nodeType = ASTNodeType::Variable;
}

void Variable::Print(std::ostream& stream, int indent) const
{
    stream << std::string(indent, ' ') << "<Variable>:\n";
    stream << std::string(indent, ' ') << "Name: " << _name;
}

Codegen::Value* Variable::Generate(Codegen::Module&, Codegen::Builder& builder)
{
    return builder.CreateLoad(namedValues.at(_name));
}

std::string Variable::GetName() const
{
    return _name;
}