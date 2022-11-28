#ifndef VIPER_AST_CALL_HH
#define VIPER_AST_CALL_HH
#include <parsing/ast/astNode.hh>
#include <lexing/token.hh>

namespace Parsing
{
    class CallExpr : public ASTNode
    {
    friend class UnaryExpression;
    public:
        CallExpr(std::unique_ptr<ASTNode> callee, std::vector<std::unique_ptr<ASTNode>> args);

        void Print(std::ostream& stream, int indent) const override;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::unique_ptr<ASTNode> _callee;
        std::vector<std::unique_ptr<ASTNode>> _args;
    };
}

#endif