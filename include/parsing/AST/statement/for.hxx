#ifndef VIPER_PARSING_AST_FOR_HXX
#define VIPER_PARSING_AST_FOR_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class ForStatement : public ASTNode
        {
        public:
            ForStatement(std::unique_ptr<ASTNode> init, std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> iter, std::unique_ptr<ASTNode> body, std::shared_ptr<Environment> scope);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope) override;
        private:
            std::unique_ptr<ASTNode> _init;
            std::unique_ptr<ASTNode> _cond;
            std::unique_ptr<ASTNode> _iter;

            std::unique_ptr<ASTNode> _body;
            std::shared_ptr<Environment> _scope;
        };
    }
}

#endif