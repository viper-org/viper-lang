#ifndef VIPER_AST_IF_HH
#define VIPER_AST_IF_HH
#include <parsing/ast/astNode.hh>

namespace Parsing
{
    class IfStatement : public ASTNode
    {
    public:
        IfStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> body, std::unique_ptr<ASTNode> elseBody);

        void Print(std::ostream& stream, int indent) const override;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::unique_ptr<ASTNode> _cond;
        std::unique_ptr<ASTNode> _body;
        std::unique_ptr<ASTNode> _elseBody;
    };
}

#endif