#ifndef VIPER_PARSING_AST_IF_HXX
#define VIPER_PARSING_AST_IF_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class IfStatement : public ASTNode
        {
        public:
            IfStatement(std::unique_ptr<ASTNode> cond,
            std::shared_ptr<Environment> scope, std::unique_ptr<ASTNode> body,
            std::shared_ptr<Environment> elseScope, std::unique_ptr<ASTNode> elseBody);
            
            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope) override;
        private:
            std::unique_ptr<ASTNode> _cond;

            std::shared_ptr<Environment> _scope;
            std::unique_ptr<ASTNode> _body;

            std::shared_ptr<Environment> _elseScope;
            std::unique_ptr<ASTNode> _elseBody;
        };
    }
}

#endif