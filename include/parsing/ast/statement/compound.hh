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
        CompoundStatement(std::vector<std::unique_ptr<ASTNode>>& statements);

        void Print(std::ostream& stream, int indent) const override;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder) override;
    private:
        std::vector<std::unique_ptr<ASTNode>> _statements;
    };
}

#endif