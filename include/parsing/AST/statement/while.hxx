#ifndef VIPER_PARSING_AST_WHILE_HXX
#define VIPER_PARSING_AST_WHILE_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class WhileStatement : public ASTNode
        {
        public:
            WhileStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> body, std::shared_ptr<Environment> scope);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        private:
            std::unique_ptr<ASTNode> _cond;

            std::unique_ptr<ASTNode> _body;
            std::shared_ptr<Environment> _scope;
        };
    }
}

#endif