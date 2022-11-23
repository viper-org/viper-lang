#ifndef VIPER_AST_STRING_HH
#define VIPER_AST_STRING_HH
#include <parsing/ast/astNode.hh>

namespace Parsing
{
    class StringLiteral : public ASTNode
    {
    public:
        StringLiteral(const std::string& value);

        void Print(std::ostream& stream, int indent) const override;

        std::string GetValue() const;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::string _value;
    };
}

#endif