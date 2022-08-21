#ifndef VIPER_PARSING_AST_CALL_HXX
#define VIPER_PARSING_AST_CALL_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class CallExpression : public ASTNode
        {
        public:
            CallExpression(std::string callee, std::vector<std::unique_ptr<ASTNode>> args);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope) override;
        private:
            std::string _callee;
            std::vector<std::unique_ptr<ASTNode>> _args;
        };
    }
}

#endif