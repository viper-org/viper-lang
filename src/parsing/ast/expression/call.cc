#include <parsing/ast/expression/call.hh>
#include <environment.hh>

namespace Parsing
{
    CallExpr::CallExpr(const std::string& callee, std::vector<std::unique_ptr<ASTNode>>& args)
        :ASTNode(ASTNodeType::Call), _callee(callee), _args(std::move(args))
    {
    }

    void CallExpr::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Call>: " << _callee;
    }

    SSA::Value* CallExpr::Emit(SSA::Builder& builder, Environment* scope, bool isStatement)
    {
        SSA::Function* function = builder.GetModule().GetFunction(_callee);
        if(function)
        {
            std::vector<SSA::Value*> args;
            for(std::unique_ptr<ASTNode>& arg : _args)
                args.push_back(arg->Emit(builder, scope));
            return builder.CreateCall(function, args, isStatement);
        }
        
        return nullptr;
    }
}