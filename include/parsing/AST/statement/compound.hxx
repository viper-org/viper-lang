#ifndef VIPER_PARSING_AST_COMPOUND_HXX
#define VIPER_PARSING_AST_COMPOUND_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class CompoundStatement : public ASTNode
        {
        public:
            CompoundStatement(std::vector<std::unique_ptr<ASTNode>> statements, std::shared_ptr<Environment> scope);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        private:
            std::vector<std::unique_ptr<ASTNode>> _statements;
            std::shared_ptr<Environment> _scope;
        };
    }
}

#endif