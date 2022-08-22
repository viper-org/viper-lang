#ifndef VIPER_PARSING_AST_RETURN_HXX
#define VIPER_PARSING_AST_RETURN_HXX
#include <parsing/AST/astNode.hxx>
#include <memory>

namespace Viper
{
    namespace Parsing
    {
        class ReturnStatement : public ASTNode
        {
        public:
            ReturnStatement(std::unique_ptr<ASTNode> value);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        private:
            std::unique_ptr<ASTNode> _value;
        };
    }
}

#endif  