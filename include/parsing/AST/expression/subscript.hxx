#ifndef VIPER_PARSING_AST_SUBSCRIPT_HXX
#define VIPER_PARSING_AST_SUBSCRIPT_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class SubscriptExpression : public ASTNode
        {
        public:
            SubscriptExpression(std::unique_ptr<ASTNode> operand, std::unique_ptr<ASTNode> index);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        private:
            std::unique_ptr<ASTNode> _operand;
            std::unique_ptr<ASTNode> _index;
        };
    }
}

#endif