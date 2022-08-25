#include <parsing/ast/topLevel/function.hh>

ASTFunction::ASTFunction(std::string name, std::vector<std::unique_ptr<ASTNode>> body)
    :_name(name), _body(std::move(body))
{
}

void ASTFunction::Print(std::ostream& stream) const
{
    stream << "<Function>:\nName:" << _name << "\nBody:\n";
    for(const std::unique_ptr<ASTNode>& node : _body)
    {
        node->Print(stream, 2);
        stream << "\n";
    }
}