#ifndef VIPER_PARSING_AST_BREAK_HXX
#define VIPER_PARSING_AST_BREAK_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class BreakStatement : public ASTNode
        {
        public:
            BreakStatement();

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        };
    }
}

#endif