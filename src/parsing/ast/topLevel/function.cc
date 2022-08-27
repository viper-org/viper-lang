#include <parsing/ast/topLevel/function.hh>
#include <codegen/value/global/function.hh>

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

Codegen::Global* ASTFunction::Generate(Codegen::Module& module, Codegen::Builder& builder)
{
    Codegen::Function* function = Codegen::Function::Create(_name, module);

    Codegen::BasicBlock* entryBlock = Codegen::BasicBlock::Create(module, _name, function);
    builder.SetInsertPoint(entryBlock);

    for(std::unique_ptr<ASTNode>& node : _body)
    {
        node->Generate(module, builder);
        if(node->GetNodeType() == ASTNodeType::ReturnStatement)
            break;
    }

    return function;
}