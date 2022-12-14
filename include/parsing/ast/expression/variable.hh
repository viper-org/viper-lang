#ifndef VIPER_AST_EXPRESSION_VARIABLE_HH
#define VIPER_AST_EXPRESSION_VARIABLE_HH
#include <parsing/ast/astNode.hh>

namespace Parsing
{
    class Variable : public ASTNode
    {
    public:
        Variable(std::string name, std::shared_ptr<Type> type);

        void Print(std::ostream& stream, int indent) const override;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;

        std::string GetName() const;
    private:
        std::string _name;
    };
}

#endif