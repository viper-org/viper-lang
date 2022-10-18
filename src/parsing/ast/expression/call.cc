#include <parsing/ast/expression/call.hh>
#include <environment.hh>

namespace Parsing
{
    CallExpr::CallExpr(const std::string& callee)
        :ASTNode(ASTNodeType::Variable), _callee(callee)
    {
    }

    void CallExpr::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Call>: " << _callee;
    }

    SSA::Value* CallExpr::Emit(SSA::Builder& builder)
    {
        SSA::Function* function = builder.GetModule().GetFunction(_callee);
        if(function)
            return builder.CreateCall(function);
        
        return nullptr;
    }
}