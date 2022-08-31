#include <parsing/ast/expression/call.hh>
#include <globals.hh>

CallExpr::CallExpr(const std::string& name, std::vector<std::unique_ptr<ASTNode>> args)
    :_callee(name), _args(std::move(args))
{
    _nodeType = ASTNodeType::Variable;
}

void CallExpr::Print(std::ostream& stream, int indent) const
{
    stream << std::string(indent, ' ') << "<Call-Expr>:\n";
    stream << std::string(indent, ' ') << "Callee: " << _callee;
}

Codegen::Value* CallExpr::Generate(Codegen::Module& module, Codegen::Builder& builder, bool isStatement)
{
    Codegen::Function* function = module.GetFunction(_callee);

    std::vector<Codegen::Value*> argValues;
    for(std::unique_ptr<ASTNode>& arg : _args)
        argValues.push_back(arg->Generate(module, builder));

    if(function)
        return builder.CreateCall(function, argValues, isStatement);

    return nullptr;
}