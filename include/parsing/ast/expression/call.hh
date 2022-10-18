#ifndef VIPER_AST_EXPRESSION_CALL_HH
#define VIPER_AST_EXPRESSION_CALL_HH
#include <parsing/ast/astNode.hh>

namespace Parsing
{
    class CallExpr : public ASTNode
    {
    public:
        CallExpr(const std::string& callee);

        void Print(std::ostream& stream, int indent) const override;

        SSA::Value* Emit(SSA::Builder& builder) override;
    private:
        std::string _callee;
    };
}

#endif