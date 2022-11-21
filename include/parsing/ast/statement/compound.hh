#ifndef VIPER_AST_STATEMENT_COMPOUND_HH
#define VIPER_AST_STATEMENT_COMPOUND_HH
#include <parsing/ast/astNode.hh>
#include <memory>
#include <vector>

namespace Parsing
{
    class CompoundStatement : public ASTNode
    {
    public:
        CompoundStatement(std::vector<std::unique_ptr<ASTNode>>& statements, std::shared_ptr<Environment> scope);

        void Print(std::ostream& stream, int indent) const override;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::vector<std::unique_ptr<ASTNode>> _statements;
        std::shared_ptr<Environment> _scope;
    };
}

#endif